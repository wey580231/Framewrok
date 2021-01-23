/*!
 *  @brief     窗体动画控制
 *  @details   控制widget属性动画变换
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.28
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef WIDGETANIMATION_H
#define WIDGETANIMATION_H

#include <QObject>
#include <QMap>
#include <QPropertyAnimation>
#include <QVariantAnimation>
#include <QSharedPointer>
#include <functional>

#include "../base_global.h"

namespace Base {

typedef std::function<void()> AnimationFinishedCallBack;
typedef std::function<void(QVariant)> VariantChangedCallBack;

class RVariantAnimation : public QVariantAnimation
{
    Q_OBJECT
public:
    RVariantAnimation(QObject * parent = nullptr);
    ~RVariantAnimation(){}

    void setCallBack(VariantChangedCallBack callback = nullptr);

    void setId(QString id){m_id = id;}
    QString getId(){return m_id;}

protected:
    void updateCurrentValue(const QVariant &value) override;

private:
    VariantChangedCallBack m_callback;

    QString m_id;
};

class BASESHARED_EXPORT WidgetAnimation : public QObject
{
    Q_OBJECT
public:
    enum Type {
        Linear,
        InQuad, OutQuad, InOutQuad, OutInQuad,
        InCubic, OutCubic, InOutCubic, OutInCubic,
        InQuart, OutQuart, InOutQuart, OutInQuart,
        InQuint, OutQuint, InOutQuint, OutInQuint,
        InSine, OutSine, InOutSine, OutInSine,
        InExpo, OutExpo, InOutExpo, OutInExpo,
        InCirc, OutCirc, InOutCirc, OutInCirc,
        InElastic, OutElastic, InOutElastic, OutInElastic,
        InBack, OutBack, InOutBack, OutInBack,
        InBounce, OutBounce, InOutBounce, OutInBounce,
        InCurve, OutCurve, SineCurve, CosineCurve,
        BezierSpline, TCBSpline, Custom, NCurveTypes
    };

    WidgetAnimation();

    /*!
     * @brief 添加基于位置及尺寸的动画
     */
    void addAnimation(QWidget * targetObj,QRect finalGeometry,AnimationFinishedCallBack call = nullptr,int duration = 350,Type type = OutCubic);

    /*!
     * @brief 添加基于某个变量变化过程的动画
     * @details 可设置起始和终止值，在变化过程中会调用回调函数
     */
    void addVariantAnimation(QVariant startValue,QVariant endValue,VariantChangedCallBack callback = nullptr,int duration = 350,AnimationFinishedCallBack finishCallback = nullptr,Type type = OutCubic);

private slots:
    void removeWidget();
    void variCallback();

private:
    void abort(QWidget *widget);

private:

    template<class T,class V>
    struct AnimInfo{
        AnimInfo(){callBack = nullptr;}
        QSharedPointer<T> pAnimation;
        V callBack;
    };
    typedef QMap<QWidget *,AnimInfo<QPropertyAnimation,AnimationFinishedCallBack>> AnimationMap;
    AnimationMap m_animation_map;

    typedef QMap<QString,AnimInfo<RVariantAnimation,AnimationFinishedCallBack>> VariAnimationMap;
    VariAnimationMap m_variAnimation_map;

};

} //namepace Base

#endif // WIDGETANIMATION_H
