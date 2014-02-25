/*
 * Cantata
 *
 * Copyright (c) 2011-2012 Craig Drummond <craig.p.drummond@gmail.com>
 *
 */
/****************************************************************************************
 * Copyright (c) 2010 Téo Mrnjavac <teo@kde.org>                                        *
 *                                                                                      *
 * This program is free software; you can redistribute it and/or modify it under        *
 * the terms of the GNU General Public License as published by the Free Software        *
 * Foundation; either version 2 of the License, or (at your option) any later           *
 * version.                                                                             *
 *                                                                                      *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY      *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.             *
 *                                                                                      *
 * You should have received a copy of the GNU General Public License along with         *
 * this program.  If not, see <http://www.gnu.org/licenses/>.                           *
 ****************************************************************************************/

#include "valueslider.h"
#include "KDE/KLocalizedString"
#include <QtGui/QHBoxLayout>

ValueSlider::ValueSlider(QWidget *parent)
    : QWidget(parent)
{
    defaultSetting=0;
    QBoxLayout *mainLayout = new QVBoxLayout(this);
    QBoxLayout *secondaryTopLayout = new QHBoxLayout(this);
    QBoxLayout *secondaryBotLayout = new QHBoxLayout(this);
    valueTypeLabel = new QLabel(this);
    valueTypeLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mainLayout->addWidget(valueTypeLabel);
    mainLayout->addLayout(secondaryTopLayout);
    mainLayout->addLayout(secondaryBotLayout);
    secondaryTopLayout->addSpacing(4);
    slider = new QSlider(this);
    slider->setOrientation(Qt::Horizontal);
    slider->setTickPosition(QSlider::TicksBelow);
    slider->setTickInterval(1);
    slider->setPageStep(2);
    slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    secondaryTopLayout->addWidget(slider, 3);
    leftLabel = new QLabel( this);
    secondaryBotLayout->addWidget(leftLabel, 1);
    midLabel = new QLabel(this);
    connect(slider, SIGNAL(valueChanged(int)),  this, SLOT(onSliderChanged(int)));
    rightLabel = new QLabel(this);
    rightLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    secondaryBotLayout->addWidget(rightLabel, 1);
    mainLayout->addWidget(midLabel);
    valueTypeLabel->setBuddy(slider);
    midLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

void ValueSlider::setValues(const Encoders::Encoder &enc)
{
    slider->setToolTip(enc.tooltip);
    valueTypeLabel->setToolTip(enc.tooltip);
    slider->setWhatsThis(enc.tooltip);
    valueTypeLabel->setWhatsThis(enc.tooltip);
    leftLabel->setText(QString());
    midLabel->setText(QString());
    rightLabel->setText(QString());
    valueTypeLabel->setText(QString());
    settings=enc.values;
    defaultSetting=0;
    if (enc.values.count()>1) {
        defaultSetting=enc.defaultValueIndex;
        valueTypeLabel->setText(enc.valueLabel);
        slider->setRange(0, enc.values.count()-1);
        slider->setValue(defaultSetting);
        onSliderChanged(defaultSetting);
        leftLabel->setText(enc.low);
        rightLabel->setText(enc.high);
    }
    onSliderChanged(defaultSetting);
}

void ValueSlider::setValue(int value)
{
    if (settings.count()>1) {
        bool increase=settings.at(0).value<settings.at(1).value;
        int index=0;
        foreach (const Encoders::Setting &s, settings) {
            if ((increase && s.value>=value) || (!increase && s.value<=value)) {
                break;
            } else {
                index++;
            }
        }
        slider->setValue(index);
    }
}

void ValueSlider::onSliderChanged(int value)
{
    QString text=value<settings.count() ? settings.at(value).descr : QString();

    if (value==defaultSetting)
        text += i18n(" (recommended)");

    midLabel->setText(text);
    emit valueChanged(value);
}