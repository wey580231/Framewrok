#include "widgetanimation.h"

#include <QWidget>
#include <QRect>
#include <QDebug>
#include "rutil.h"

namespace Base{

RVariantAnimation::RVariantAnimation(QObject* parent):QVariantAnimation(parent),m_callback(nullptr)
{

}

void RVariantAnimation::setCallBack(VariantChangedCallBack callback)
{
    m_callback = callback;
}

void RVariantAnimation::updateCurrentValue(const QVariant& value)
{
    if(m_callback)
        m_callback(value);
}

WidgetAnimation::WidgetAnimation()
{

}

/*!
 * @brief 对目标的尺寸添加动画
 * @param[in] targetObj 待执行动画的基类
 * @param[in] finalGeometry 目的位置尺寸
 * @param[in] callback 动画结束后执行的回调函数
 * @param[in] type 动画类型
 */
void WidgetAnimation::addAnimation(QWidget *targetObj, QRect finalGeometry, AnimationFinishedCallBack callback, int duration, Type type)
{
    AnimationMap::const_iterator it = m_animation_map.constFind(targetObj);
    if (it != m_animation_map.constEnd() && (*it).pAnimation->endValue().toRect() == finalGeometry)
        return;

    QSharedPointer<QPropertyAnimation> animation = QSharedPointer<QPropertyAnimation>(new QPropertyAnimation(targetObj,"geometry"));
    connect(animation.data(),SIGNAL(finished()),this,SLOT(removeWidget()));
    animation->setEndValue(finalGeometry);
    animation->setDuration(duration);
    animation->setEasingCurve(static_cast<QEasingCurve::Type>(type));
    animation->start(QPropertyAnimation::DeleteWhenStopped);

    AnimInfo<QPropertyAnimation,AnimationFinishedCallBack> animinfo;
    animinfo.pAnimation = animation;
    animinfo.callBack = callback;
    m_animation_map.insert(targetObj,animinfo);
}

/*!
 * @brief 添加变量变化的动画
 * @param[in] startValue 起始变量值
 * @param[in] endValue 终止变量值
 * @param[in] callback 动画结束后执行的回调函数
 * @param[in] duration 整个动画持续时间
 * @param[in] finishCallback 动画结束后回调
 * @param[in] type 动画效果类型
 */
void WidgetAnimation::addVariantAnimation(QVariant startValue, QVariant endValue, VariantChangedCallBack callback, int duration,AnimationFinishedCallBack finishCallback,Type type)
{
    QSharedPointer<RVariantAnimation> animation(new RVariantAnimation);
    connect(animation.data(),SIGNAL(finished()),this,SLOT(variCallback()));

    animation->setCallBack(callback);
    animation->setDuration(duration);
    animation->setEasingCurve(static_cast<QEasingCurve::Type>(type));
    animation->setStartValue(startValue);
    animation->setEndValue(endValue);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    animation->setId(RUtil::UUID());

    AnimInfo<RVariantAnimation,AnimationFinishedCallBack> animinfo;
    animinfo.pAnimation = animation;
    animinfo.callBack = finishCallback;
    m_variAnimation_map.insert(animation->getId(),animinfo);
}


/*!
 * @brief 动画执行结束后，执行回调函数
 */
void WidgetAnimation::removeWidget()
{
    QPropertyAnimation * target = dynamic_cast<QPropertyAnimation *>(QObject::sender());
    AnimationMap::iterator  iter = m_animation_map.begin();
    while(iter != m_animation_map.end()){
        if(iter.value().pAnimation.data() == target){
            if(iter.value().callBack != nullptr){
                iter.value().callBack();
            }
            break;
        }
        iter++;
    }

    abort(static_cast<QWidget*>(target->targetObject()));
}

/*!
 * @brief 基于变量的动画结束回调
 */
void WidgetAnimation::variCallback()
{
    RVariantAnimation * target = dynamic_cast<RVariantAnimation *>(QObject::sender());
    if(target && m_variAnimation_map.contains(target->getId())){
        QString uuid = target->getId();
        if(m_variAnimation_map.value(uuid).callBack){
            m_variAnimation_map.value(uuid).callBack();
        }

        m_variAnimation_map.remove(uuid);
    }
}

void WidgetAnimation::abort(QWidget *widget)
{
    AnimationMap::iterator it = m_animation_map.find(widget);
    if (it == m_animation_map.end())
        return;

    QSharedPointer<QPropertyAnimation> anim = it.value().pAnimation;
    m_animation_map.erase(it);
    if(anim){
        anim->stop();
    }
}

} //namespace Base
