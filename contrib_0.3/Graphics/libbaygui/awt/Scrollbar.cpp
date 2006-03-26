/*
Copyright (c) 2005 bayside

Permission is hereby granted, free of charge, to any person 
obtaining a copy of this software and associated documentation files 
(the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, 
publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be 
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "baygui.h"

namespace baygui {
	/** 垂直ボタン（パレット）*/
	static dword button_palette[] = {
		0xff040204,
		0xff8486dc,
		0xffdcdafc,
		0xff5456b4,
		0xffb4b2fc,
		0xfff4f2f4,
	};

	/** 垂直ボタン（データ）*/
	static unsigned char button_data[] = {
		0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
		0x0,0x5,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x4,0x0,
		0x0,0x2,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x1,0x0,
		0x0,0x2,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x1,0x0,
		0x0,0x2,0x4,0x4,0x5,0x2,0x2,0x2,0x2,0x2,0x2,0x4,0x4,0x4,0x1,0x0,
		0x0,0x2,0x4,0x4,0x4,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x4,0x1,0x0,
		0x0,0x2,0x4,0x4,0x5,0x2,0x2,0x2,0x2,0x2,0x2,0x4,0x4,0x4,0x1,0x0,
		0x0,0x2,0x4,0x4,0x4,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x4,0x1,0x0,
		0x0,0x2,0x4,0x4,0x5,0x2,0x2,0x2,0x2,0x2,0x2,0x4,0x4,0x4,0x1,0x0,
		0x0,0x2,0x4,0x4,0x4,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x4,0x1,0x0,
		0x0,0x2,0x4,0x4,0x5,0x2,0x2,0x2,0x2,0x2,0x2,0x4,0x4,0x4,0x1,0x0,
		0x0,0x2,0x4,0x4,0x4,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x4,0x1,0x0,
		0x0,0x2,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x1,0x0,
		0x0,0x2,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x1,0x0,
		0x0,0x2,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x1,0x0,
		0x0,0x4,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x0,
		0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	};

	/** 上向き矢印（パレット）*/
	static dword arrow_palette[] = {
		0xff040204,
		0xffcccecc,
		0xfffcfefc,
		0xffe4e6e4,
	};

	/** 上向き矢印（データ）*/
	static unsigned char arrow1_data[] = {
		0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
		0x0,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x3,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x3,0x0,0x0,0x3,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x0,0x0,0x0,0x0,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x0,0x0,0x0,0x0,0x0,0x0,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x3,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x0,
		0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	};

	/** 下向き矢印（データ）*/
	static unsigned char arrow2_data[] = {
		0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
		0x0,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x3,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x0,0x0,0x0,0x0,0x0,0x0,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x0,0x0,0x0,0x0,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x3,0x0,0x0,0x3,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x2,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x1,0x0,
		0x0,0x3,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x0,
		0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	};

	Scrollbar::Scrollbar()
	{
		this->orientation = VERTICAL;
		this->minimum = 0;
		this->maximum = 100;
		this->blocksize = 10;
		this->preValue = 0;
		this->value = 0;
		this->adjustmentEvent.setType(Event::BLOCK_INCLEMENT);
		this->adjustmentEvent.setSource(this);
	}

	Scrollbar::Scrollbar(int orientation)
	{
		this->orientation = orientation;
		this->minimum = 0;
		this->maximum = 100;
		this->blocksize = 10;
		this->preValue = 0;
		this->value = 0;
		this->adjustmentEvent.setType(Event::BLOCK_INCLEMENT);
		this->adjustmentEvent.setSource(this);
	}

	Scrollbar::~Scrollbar()
	{
	}

	void Scrollbar::setValue(int value)
	{
		if (value < this->minimum) {
			this->value = this->minimum;
		} else if (value > this->maximum) {
			this->value = this->maximum;
		} else {
			this->value = value;
		}
		repaint();
		if (value < this->preValue) {
			this->adjustmentEvent.setType(Event::BLOCK_DECLEMENT);
			getParent()->processEvent(&this->adjustmentEvent);
		} else {
			this->adjustmentEvent.setType(Event::BLOCK_INCLEMENT);
			getParent()->processEvent(&this->adjustmentEvent);
		}
		this->preValue = value;
	}

	void Scrollbar::setBounds(int x, int y, int w, int h)
	{
		// 幅と高さは16固定とする
		(this->orientation == VERTICAL) ? Component::setBounds(x, y, 16, h) : Component::setBounds(x, y, w, 16);
	}

	void Scrollbar::paint(Graphics* g)
	{
		int w = getWidth(), h = getHeight();
		
		// 外枠
		g->setColor(Color::gray);
		g->fillRect(0,0,getWidth(),getHeight());
		g->setColor(Color::black);
		g->drawRect(0,0,getWidth(),getHeight());
		
		// 垂直スクロールバー
		if (this->orientation == VERTICAL) {
			// 上向き矢印
			for (int i = 0; i < 16; i++) {
				for (int j = 0; j < 16; j++) {
					g->drawPixel(j, i, arrow_palette[arrow1_data[i * 16 + j] & 0xFF]);
				}
			}
			// 下向き矢印
			for (int i = 0; i < 16; i++) {
				for (int j = 0; j < 16; j++) {
					g->drawPixel(j, i + h - 16, arrow_palette[arrow2_data[i * 16 + j] & 0xFF]);
				}
			}
			// ボタン
			int offset = 15 + (getHeight() - 47) * (getValue() - getMinimum()) / (getMaximum() - getMinimum());
			for (int i = 0; i < 17; i++) {
				for (int j = 0; j < 16; j++) {
					g->drawPixel(j, i + offset, button_palette[button_data[i * 16 + j] & 0xFF]);
				}
			}
		// 水平スクロールバー
		} else {
			// 左向き矢印
			for (int i = 0; i < 16; i++) {
				for (int j = 0; j < 16; j++) {
					g->drawPixel(i, j, arrow_palette[arrow1_data[i * 16 + j] & 0xFF]);
				}
			}
			// 右向き矢印
			for (int i = 0; i < 16; i++) {
				for (int j = 0; j < 16; j++) {
					g->drawPixel(i + w - 16, j, arrow_palette[arrow2_data[i * 16 + j] & 0xFF]);
				}
			}
			// ボタン
			int offset = 15 + (getWidth() - 47) * (getValue() - getMinimum()) / (getMaximum() - getMinimum());
			for (int i = 0; i < 17; i++) {
				for (int j = 0; j < 16; j++) {
					g->drawPixel(i + offset, j, button_palette[button_data[i * 16 + j] & 0xFF]);
				}
			}
		}
	}

	void Scrollbar::processEvent(Event* event)
	{
		// 非活性の時はイベントを受け付けない
		if (getEnabled() == false) return;
		if (event->getType() == MouseEvent::MOUSE_PRESSED) {
			MouseEvent* me = (MouseEvent *)event;
			int mx = me->getX();
			int my = me->getY();
			// 垂直スクロールバー
			if (this->orientation == VERTICAL) {
				if (0 < my && my < 16) {
					setValue(this->value - this->blocksize);
				} else if (getHeight() - 16 < my && my < getHeight()) {
					setValue(this->value + this->blocksize);
				}
			// 水平スクロールバー
			} else {
				if (0 < mx && mx < 16) {
					setValue(this->value - this->blocksize);
				} else if (getWidth() - 16 < mx && mx < getWidth()) {
					setValue(this->value + this->blocksize);
				}
			}
		} else if (event->getType() == MouseEvent::MOUSE_DRAGGED) {
			MouseEvent* me = (MouseEvent *)event;
			int mx = me->getX();
			int my = me->getY();
			// 垂直スクロールバー
			if (this->orientation == VERTICAL && 16 <= my && my <= getHeight() - 16) {
				setValue((my - 16) * (getMaximum() - getMinimum()) / (getHeight() - 32) + getMinimum());
			// 水平スクロールバー
			} else if (this->orientation == HORIZONTAL && 16 <= mx && mx <= getWidth() - 16) {
				setValue((mx - 16) * (getMaximum() - getMinimum()) / (getWidth() - 32) + getMinimum());
			}
		}
	}
}
