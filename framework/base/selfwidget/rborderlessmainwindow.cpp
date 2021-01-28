#include "rborderlessmainwindow.h"

#include <dwmapi.h>
#include <windows.h>
#include <windowsx.h>

#include <cmath>

#include <QLayout>
#include <QApplication>
#include <QMouseEvent>
#include <QSettings>
#include <QPushButton>
#include <QDebug>

#pragma comment(lib, "dwmapi.lib")

namespace Base {

	// we cannot just use WS_POPUP style
	// WS_THICKFRAME: without this the window cannot be resized and so aero snap, de-maximizing and minimizing won't work
	// WS_SYSMENU: enables the context menu with the move, close, maximize, minimize... commands (shift + right-click on the task bar item)
	// WS_CAPTION: enables aero minimize animation/transition
	// WS_MAXIMIZEBOX, WS_MINIMIZEBOX: enable minimize/maximize
	enum class Style : DWORD {
		windowed = WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		aero_borderless = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
		basic_borderless = WS_POPUP | WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX
	};

	auto maximized(HWND hwnd) -> bool {
		WINDOWPLACEMENT placement;
		if (!::GetWindowPlacement(hwnd, &placement)) {
			return false;
		}

		return placement.showCmd == SW_MAXIMIZE;
	}

	/* Adjust client rect to not spill over monitor edges when maximized.
	* rect(in/out): in: proposed window rect, out: calculated client rect
	* Does nothing if the window is not maximized.
	*/
	auto adjust_maximized_client_rect(HWND window, RECT& rect) -> void {
		if (!maximized(window)) {
			return;
		}

		auto monitor = ::MonitorFromWindow(window, MONITOR_DEFAULTTONULL);
		if (!monitor) {
			return;
		}

		MONITORINFO monitor_info{};
		monitor_info.cbSize = sizeof(monitor_info);
		if (!::GetMonitorInfoW(monitor, &monitor_info)) {
			return;
		}

		// when maximized, make the client area fill just the monitor (without task bar) rect,
		// not the whole window rect which extends beyond the monitor.
		rect = monitor_info.rcWork;
	}

	auto composition_enabled() -> bool {
		auto composition_enabled = FALSE;
		auto success = ::DwmIsCompositionEnabled(&composition_enabled) == S_OK;
		return composition_enabled && success;
	}

	auto select_borderless_style() -> Style {
		return composition_enabled() ? Style::aero_borderless : Style::basic_borderless;
	}

	RBorderLessMainWindow::RBorderLessMainWindow(bool titleBarHoverWindow, QWidget *parent)
		:m_titleBarHoverWindow(titleBarHoverWindow), QMainWindow(parent), m_customMenuWidget(nullptr)
	{
		setWindowFlags(Qt::FramelessWindowHint);
		set_borderless(true);

		auto pal = palette();
		pal.setColor(QPalette::Background, Qt::white);
		setAutoFillBackground(true);
		setPalette(pal);

		init();
		setBorderShadowVisible(true);
		setButtonEnable(AllButt, true);
	}

	bool RBorderLessMainWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
	{
		Q_UNUSED(eventType);

		auto msg = static_cast<MSG *>(message);

		switch (msg->message)
		{
			case WM_NCCREATE: {
				auto userdata = reinterpret_cast<CREATESTRUCTW*>(msg->lParam)->lpCreateParams;
				// store window instance pointer in window user data
				::SetWindowLongPtrW(msg->hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(userdata));
			}

			case WM_ERASEBKGND:
			{
				auto brush = CreateSolidBrush(RGB(48, 48, 48));
				SetClassLongPtr(msg->hwnd, GCLP_HBRBACKGROUND, reinterpret_cast<LONG_PTR>(brush));
			}

			case WM_NCCALCSIZE: {
				if (msg->wParam == TRUE) {
					auto& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
					adjust_maximized_client_rect(msg->hwnd, params.rgrc[0]);
				}
				*result = 0;
				return true;
			}

			case WM_NCHITTEST: {
				*result = 0;
				const LONG border_width = 8; //in pixels
				RECT winrect;
				GetWindowRect(reinterpret_cast<HWND>(winId()), &winrect);

				long x = GET_X_LPARAM(msg->lParam);
				long y = GET_Y_LPARAM(msg->lParam);

				auto resizeWidth = minimumWidth() != maximumWidth();
				auto resizeHeight = minimumHeight() != maximumHeight();

				if (resizeWidth)
				{
					//left border
					if (x >= winrect.left && x < winrect.left + border_width)
					{
						*result = HTLEFT;
					}
					//right border
					if (x < winrect.right && x >= winrect.right - border_width)
					{
						*result = HTRIGHT;
					}
				}
				if (resizeHeight)
				{
					//bottom border
					if (y < winrect.bottom && y >= winrect.bottom - border_width)
					{
						*result = HTBOTTOM;
					}
					//top border
					if (y >= winrect.top && y < winrect.top + border_width)
					{
						*result = HTTOP;
					}
				}
				if (resizeWidth && resizeHeight)
				{
					//bottom left corner
					if (x >= winrect.left && x < winrect.left + border_width &&
						y < winrect.bottom && y >= winrect.bottom - border_width)
					{
						*result = HTBOTTOMLEFT;
					}
					//bottom right corner
					if (x < winrect.right && x >= winrect.right - border_width &&
						y < winrect.bottom && y >= winrect.bottom - border_width)
					{
						*result = HTBOTTOMRIGHT;
					}
					//top left corner
					if (x >= winrect.left && x < winrect.left + border_width &&
						y >= winrect.top && y < winrect.top + border_width)
					{
						*result = HTTOPLEFT;
					}
					//top right corner
					if (x < winrect.right && x >= winrect.right - border_width &&
						y >= winrect.top && y < winrect.top + border_width)
					{
						*result = HTTOPRIGHT;
					}
				}

				if (*result != 0)
					return true;

				auto action = QApplication::widgetAt(QCursor::pos());
				if (action == m_titleBarWidget) {
					*result = HTCAPTION;
					return true;
				}
				break;
			}

			case WM_GETMINMAXINFO: {
				MINMAXINFO* mmi = reinterpret_cast<MINMAXINFO*>(msg->lParam);

				if (maximized(msg->hwnd)) {

					RECT window_rect;

					if (!GetWindowRect(msg->hwnd, &window_rect)) {
						return false;

					}

					HMONITOR monitor = MonitorFromRect(&window_rect, MONITOR_DEFAULTTONULL);
					if (!monitor) {
						return false;
					}

					MONITORINFO monitor_info = { 0 };
					monitor_info.cbSize = sizeof(monitor_info);
					GetMonitorInfo(monitor, &monitor_info);

					RECT work_area = monitor_info.rcWork;
					RECT monitor_rect = monitor_info.rcMonitor;

					mmi->ptMaxPosition.x = abs(work_area.left - monitor_rect.left);
					mmi->ptMaxPosition.y = abs(work_area.top - monitor_rect.top);

					mmi->ptMaxSize.x = abs(work_area.right - work_area.left);
					mmi->ptMaxSize.y = abs(work_area.bottom - work_area.top);
					mmi->ptMaxTrackSize.x = mmi->ptMaxSize.x;
					mmi->ptMaxTrackSize.y = mmi->ptMaxSize.y;

					*result = 1;
					return true;
				}
			}

			case WM_NCACTIVATE: {
				if (!composition_enabled()) {
					// Prevents window frame reappearing on window activation
					// in "basic" theme, where no aero shadow is present.
					*result = 1;
					return true;
				}
				break;
			}

			case WM_SIZE: {
				RECT winrect;
				GetClientRect(msg->hwnd, &winrect);

				WINDOWPLACEMENT wp;
				wp.length = sizeof(WINDOWPLACEMENT);
				GetWindowPlacement(msg->hwnd, &wp);
				if (this)
				{
					if (wp.showCmd == SW_MAXIMIZE)
					{
						::SetWindowPos(reinterpret_cast<HWND>(winId()), Q_NULLPTR, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
					}
				}
			}

			default: QWidget::nativeEvent(eventType, message, result);
		}

		return QWidget::nativeEvent(eventType, message, result);
	}

	bool RBorderLessMainWindow::eventFilter(QObject *o, QEvent *e)
	{
		if (o == m_centralWidget) {
			if (e->type() == QEvent::Resize) {
				m_titleBarWidget->setGeometry(0, 0, width(), 30);
			}
		}
		return QMainWindow::eventFilter(o, e);
	}

	void RBorderLessMainWindow::mousePressEvent(QMouseEvent* event)
	{
		if (event->button() == Qt::RightButton)
		{
			if (m_iconButton->underMouse())
			{
				auto hMenu = GetSystemMenu(reinterpret_cast<HWND>(winId()), FALSE);

				if (hMenu)
				{
					MENUITEMINFO mii;
					mii.cbSize = sizeof(MENUITEMINFO);
					mii.fMask = MIIM_STATE;
					mii.fType = 0;

					// update the options
					mii.fState = MF_ENABLED;
					SetMenuItemInfo(hMenu, SC_RESTORE, FALSE, &mii);
					SetMenuItemInfo(hMenu, SC_SIZE, FALSE, &mii);
					SetMenuItemInfo(hMenu, SC_MOVE, FALSE, &mii);
					SetMenuItemInfo(hMenu, SC_MAXIMIZE, FALSE, &mii);
					SetMenuItemInfo(hMenu, SC_MINIMIZE, FALSE, &mii);

					mii.fState = MF_GRAYED;

					WINDOWPLACEMENT wp;
					GetWindowPlacement(reinterpret_cast<HWND>(winId()), &wp);

					switch (wp.showCmd)
					{
						case SW_SHOWMAXIMIZED:
							SetMenuItemInfo(hMenu, SC_SIZE, FALSE, &mii);
							SetMenuItemInfo(hMenu, SC_MOVE, FALSE, &mii);
							SetMenuItemInfo(hMenu, SC_MAXIMIZE, FALSE, &mii);
							SetMenuDefaultItem(hMenu, SC_CLOSE, FALSE);
							break;
						case SW_SHOWMINIMIZED:
							SetMenuItemInfo(hMenu, SC_MINIMIZE, FALSE, &mii);
							SetMenuDefaultItem(hMenu, SC_RESTORE, FALSE);
							break;
						case SW_SHOWNORMAL:
							SetMenuItemInfo(hMenu, SC_RESTORE, FALSE, &mii);
							SetMenuDefaultItem(hMenu, SC_CLOSE, FALSE);
							break;
						default:;
					}

					POINT p;
					if (GetCursorPos(&p))
					{
						LPARAM cmd = TrackPopupMenu(hMenu, (TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
							p.x, p.y, NULL, reinterpret_cast<HWND>(winId()), Q_NULLPTR);

						if (cmd) PostMessage(reinterpret_cast<HWND>(winId()), WM_SYSCOMMAND, cmd, 0);
					}
				}
			}
		}
	}

	void RBorderLessMainWindow::changeEvent(QEvent* e)
	{
		QWidget::changeEvent(e);

		if (e->type() == QEvent::WindowStateChange)
		{
			auto ev = static_cast<QWindowStateChangeEvent*>(e);

			if (!(ev->oldState() & Qt::WindowMaximized) && windowState() & Qt::WindowMaximized)
			{
				m_restoreButton->setVisible(true);
				m_maxButton->setVisible(false);
			}
			else
			{
				m_restoreButton->setVisible(false);
				m_maxButton->setVisible(true);
			}
		}
	}

	void RBorderLessMainWindow::slot_show_system_menu() const
	{
		auto hMenu = GetSystemMenu(reinterpret_cast<HWND>(winId()), FALSE);

		if (hMenu)
		{
			MENUITEMINFO mii;
			mii.cbSize = sizeof(MENUITEMINFO);
			mii.fMask = MIIM_STATE;
			mii.fType = 0;

			// update the options
			mii.fState = MF_ENABLED;
			SetMenuItemInfo(hMenu, SC_RESTORE, FALSE, &mii);
			SetMenuItemInfo(hMenu, SC_SIZE, FALSE, &mii);
			SetMenuItemInfo(hMenu, SC_MOVE, FALSE, &mii);
			SetMenuItemInfo(hMenu, SC_MAXIMIZE, FALSE, &mii);
			SetMenuItemInfo(hMenu, SC_MINIMIZE, FALSE, &mii);

			mii.fState = MF_GRAYED;

			WINDOWPLACEMENT wp;
			GetWindowPlacement(reinterpret_cast<HWND>(winId()), &wp);

			switch (wp.showCmd)
			{
				case SW_SHOWMAXIMIZED:
					SetMenuItemInfo(hMenu, SC_SIZE, FALSE, &mii);
					SetMenuItemInfo(hMenu, SC_MOVE, FALSE, &mii);
					SetMenuItemInfo(hMenu, SC_MAXIMIZE, FALSE, &mii);
					SetMenuDefaultItem(hMenu, SC_CLOSE, FALSE);
					break;
				case SW_SHOWMINIMIZED:
					SetMenuItemInfo(hMenu, SC_MINIMIZE, FALSE, &mii);
					SetMenuDefaultItem(hMenu, SC_RESTORE, FALSE);
					break;
				case SW_SHOWNORMAL:
					SetMenuItemInfo(hMenu, SC_RESTORE, FALSE, &mii);
					SetMenuDefaultItem(hMenu, SC_CLOSE, FALSE);
					break;
				default:;
			}

			RECT winrect;
			GetWindowRect(reinterpret_cast<HWND>(winId()), &winrect);

			if (windowState() != Qt::WindowMaximized)
			{
				LPARAM cmd = TrackPopupMenu(hMenu, (TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
					winrect.left, winrect.top + 30, NULL, reinterpret_cast<HWND>(winId()), Q_NULLPTR);

				if (cmd) PostMessage(reinterpret_cast<HWND>(winId()), WM_SYSCOMMAND, cmd, 0);
			}
			else
			{
				auto monitor = MonitorFromWindow(reinterpret_cast<HWND>(winId()), MONITOR_DEFAULTTONULL);
				MONITORINFO monitor_info{};
				monitor_info.cbSize = sizeof(monitor_info);
				GetMonitorInfoW(monitor, &monitor_info);
				auto rect = monitor_info.rcWork;

				LPARAM cmd = TrackPopupMenu(hMenu, (TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
					rect.left, rect.top + 30, NULL, reinterpret_cast<HWND>(winId()), Q_NULLPTR);

				if (cmd) PostMessage(reinterpret_cast<HWND>(winId()), WM_SYSCOMMAND, cmd, 0);
			}
		}
	}

	void RBorderLessMainWindow::slot_minimized()
	{
		setWindowState(Qt::WindowMinimized);
	}

	void RBorderLessMainWindow::slot_maximized()
	{
		m_restoreButton->setVisible(true);
		m_maxButton->setVisible(false);
		setWindowState(Qt::WindowMaximized);
	}

	void RBorderLessMainWindow::slot_restored()
	{
		m_restoreButton->setVisible(false);
		m_maxButton->setVisible(true);
		setWindowState(Qt::WindowNoState);
	}

	void RBorderLessMainWindow::slot_closed()
	{
		close();
	}

	void RBorderLessMainWindow::init()
	{
		//主窗体
		m_centralWidget = new QWidget();
		m_centralWidget->installEventFilter(this);

		auto verticalLayout = new QVBoxLayout();
		verticalLayout->setAlignment(Qt::AlignTop);
		verticalLayout->setSpacing(0);
		verticalLayout->setMargin(0);
		verticalLayout->addWidget(m_centralWidget, 1);

		QWidget * twidget = new QWidget(this);
		twidget->setObjectName("RBorderLess_CentralWidget");
		twidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		twidget->setLayout(verticalLayout);
		twidget->setContentsMargins(0, 0, 0, 0);
		QMainWindow::setCentralWidget(twidget);

		auto titleBarHLayout = new QHBoxLayout();
		titleBarHLayout->setSpacing(0);
		titleBarHLayout->setMargin(0);

		m_titleBarWidget = new QWidget(this);
		m_titleBarWidget->setFixedHeight(30);
		m_titleBarWidget->setObjectName("RBorderLess_WindowTitleBar");
		m_titleBarWidget->setContentsMargins(0, 0, 0, 0);
		m_titleBarWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
		m_titleBarWidget->setLayout(titleBarHLayout);

		m_iconButton = new QPushButton(m_titleBarWidget);
		m_iconButton->setIconSize(QSize(20, 20));
		m_iconButton->setStyleSheet("margin-left: 5px; background-color: transparent; border: none;");
		m_iconButton->setObjectName("RBorderLess_TitleBarIcon");
		m_iconButton->setFocusPolicy(Qt::NoFocus);
		connect(m_iconButton, SIGNAL(clicked()), this, SLOT(slot_show_system_menu()));

		QSize fixedButtSize(36, 29);

		auto createButt = [&](QString objName, QString toolTip, const char * slotName) {
			RIconButton * butt = new RIconButton(m_titleBarWidget);
			butt->setFixedSize(fixedButtSize);
			butt->setObjectName(objName);
			butt->setToolTip(toolTip);
			butt->setFocusPolicy(Qt::NoFocus);
			butt->disableColors(RIconButton::Color_NormalBackGround | RIconButton::Color_NormalBorder
				| RIconButton::Color_HoverBackground | RIconButton::Color_HoverBorder);
			butt->enableColor(RIconButton::Color_HoverBackground, QColor(255, 255, 255, 60));
			connect(butt, SIGNAL(clicked()), this, slotName);
			return butt;
		};

		m_minButton = createButt("RBorderLess_MinimizeButton", QStringLiteral("最小化"), SLOT(slot_minimized()));
		m_maxButton = createButt("RBorderLess_MaximizeButton", QStringLiteral("最大化"), SLOT(slot_maximized()));
		m_restoreButton = createButt("RBorderLess_RestoreButton", QStringLiteral("向下还原"), SLOT(slot_restored()));
		m_closeButton = createButt("RBorderLess_CloseButton", QStringLiteral("关闭"), SLOT(slot_closed()));

		m_minButton->setIcon(QIcon(QStringLiteral(":/icon/resource/icon/Minimize.png")));
		m_minButton->setCheckedIcon(QIcon(QStringLiteral(":/icon/resource/icon/MinimizeHover.png")));

		m_maxButton->setIcon(QIcon(QStringLiteral(":/icon/resource/icon/Maximize.png")));
		m_maxButton->setCheckedIcon(QIcon(QStringLiteral(":/icon/resource/icon/MaximizeHover.png")));

		m_restoreButton->setIcon(QIcon(QStringLiteral(":/icon/resource/icon/Restore.png")));
		m_restoreButton->setCheckedIcon(QIcon(QStringLiteral(":/icon/resource/icon/RestoreHover.png")));

		m_closeButton->setIcon(QIcon(QStringLiteral(":/icon/resource/icon/Close.png")));
		m_closeButton->setCheckedIcon(QIcon(QStringLiteral(":/icon/resource/icon/CloseHover.png")));

		m_restoreButton->setVisible(false);

		titleBarHLayout->addWidget(m_iconButton);

		if (m_customMenuWidget)
			titleBarHLayout->addWidget(m_customMenuWidget);

		titleBarHLayout->addStretch(1);
		titleBarHLayout->addWidget(m_minButton);
		titleBarHLayout->addWidget(m_restoreButton);
		titleBarHLayout->addWidget(m_maxButton);
		titleBarHLayout->addWidget(m_closeButton);

		if (!m_titleBarHoverWindow) {

			verticalLayout->insertWidget(0, m_titleBarWidget, 0, Qt::AlignTop);
		}
	}

	void RBorderLessMainWindow::updateButtState()
	{
		m_iconButton->setVisible(m_buttFlags.testFlag(IconButt));
		m_minButton->setVisible(m_buttFlags.testFlag(MenuWidget));
		m_maxButton->setVisible(m_buttFlags.testFlag(MinButt));
		m_restoreButton->setVisible(m_buttFlags.testFlag(MaxButt));
		m_closeButton->setVisible(m_buttFlags.testFlag(CloseButt));
	}

	auto RBorderLessMainWindow::set_borderless(bool enabled) const -> void
	{
		auto new_style = (enabled) ? select_borderless_style() : Style::windowed;
		auto old_style = static_cast<Style>(::GetWindowLongPtrW(reinterpret_cast<HWND>(winId()), GWL_STYLE));

		if (new_style != old_style) {
			::SetWindowLongPtrW(reinterpret_cast<HWND>(winId()), GWL_STYLE, static_cast<LONG>(new_style));

			// redraw frame
			::SetWindowPos(reinterpret_cast<HWND>(winId()), nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
		}
	}

	void RBorderLessMainWindow::setCustomMenuWidget(QWidget *widget)
	{
		m_customMenuWidget = widget;
	}

	void RBorderLessMainWindow::setCentralWidget(QWidget *widget)
	{
		if (widget == nullptr)
			return;

		QHBoxLayout * hlayout = dynamic_cast<QHBoxLayout *>(m_centralWidget->layout());
		if (hlayout == nullptr) {
			hlayout = new QHBoxLayout();
			hlayout->setContentsMargins(0, 0, 0, 0);
			m_centralWidget->setLayout(hlayout);
		}

		if (hlayout->count() == 1 && hlayout->itemAt(0)->widget()) {
			delete hlayout->takeAt(0)->widget();
		}

		hlayout->addWidget(widget);
	}

	void RBorderLessMainWindow::setBorderShadowVisible(bool visible)
	{
		HWND appwnd;
		appwnd = (HWND)winId();
		const MARGINS shadow_on = { 1, 1, 1, 1 };
		const MARGINS shadow_off = { 0, 0, 0, 0 };
		DwmExtendFrameIntoClientArea(appwnd, (visible) ? (&shadow_on) : (&shadow_off));
	}

	void RBorderLessMainWindow::setButtonEnable(ButtonFlags bf, bool enabled /*= true*/)
	{
		if (enabled)
			m_buttFlags |= bf;
		else
			m_buttFlags &= ~bf;

		updateButtState();
	}

	void RBorderLessMainWindow::setProgramIcon(const QString& iconPath)
	{

	}

} //namespace Base 