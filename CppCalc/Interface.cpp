#include "Interface.h"
#include "MyString.h"

namespace Interface
{
	void SetCursor(HWND hWnd, LPCWSTR cursorName)
	{
		HCURSOR hcur = LoadCursor(NULL, cursorName);
#ifdef _WIN64
		SetClassLongPtr(hWnd, GCLP_HCURSOR, LONG_PTR(hcur));
#else
		SetClassLong(hWnd, GCL_HCURSOR, long(hcur));
#endif
	}

	Control::Control(Form& form, int left, int top, int width, int height, bool focusable, void (*onUpdateFunction)(), void (*onClickFunction)()) :
		form(form), left(left), top(top), width(width), height(height), focusable(focusable), onUpdateFunction(onUpdateFunction), onClickFunction(onClickFunction)
	{
		form.AddControl(*this);
	}

	bool Control::Check(int x, int y)
	{
		return x >= left && y >= top && x <= left + width && y <= top + height;
	}

	void Control::Update()
	{
		/// 执行用户自定义更新事件
		if (onUpdateFunction != nullptr) onUpdateFunction();
	}

	void Control::Render()
	{
	}

	void Control::OnMouseDown(int x, int y)
	{
	}

	void Control::OnMouseUp(int x, int y)
	{
		/// 执行用户自定义点击事件
		if (onClickFunction != nullptr) onClickFunction();
	}

	void Control::OnMouseMove(int x, int y)
	{
	}

	void Control::OnMouseMoveFocused(int x, int y)
	{
	}

	void Control::OnChar(wchar_t c)
	{
	}

	void Control::OnKeyDown(bool ctrl, bool shift, BYTE vkcode)
	{
	}

	void Control::OnKeyUp(bool ctrl, bool shift, BYTE vkcode)
	{
	}
	
	Form::Form(int width, int height, wstring caption, void (*onUpdateFunction)()):
		width(width), height(height), caption(caption), onUpdateFunction(onUpdateFunction) { }

	void Form::Main()
	{
		/// 初始化窗口
		hWnd = initgraph(width, height);
		
		/// 窗口标题
		SetWindowText(hWnd, caption.c_str());

		BeginBatchDraw();
		
		/// 主循环
		while (true)
		{
			Update();
			Render();
		}

		EndBatchDraw();
		closegraph();
	}

	void Form::Update()
	{
		/// 处理输入
		ExMessage msg;
		bool mouseOnWindow = false; /// 鼠标是否在窗口上
		while (peekmessage(&msg, EM_MOUSE | EM_KEY | EM_CHAR, false))
		{
			if (msg.message == WM_LBUTTONDOWN)
			{
				mouseDown = true;
				ctrl = shift = false; /// 因为焦点问题不好解决，重置ctrl和shift

				focusIndex = -1;
				for (int i = 0; i < int(controls.size()); ++i)
				{
					if (!controls[i]->focusable) continue;
					if (controls[i]->Check(msg.x, msg.y))
					{
						focusIndex = i;
						controls[i]->OnMouseDown(msg.x, msg.y);
						break;
					}
				}
			}
			else if (msg.message == WM_LBUTTONUP || msg.message == WM_MOUSELEAVE) //TODO Track WM_MOUSELEAVE?
			{
				mouseDown = false;

				for (int i = 0; i < int(controls.size()); ++i)
				{
					if (controls[i]->Check(msg.x, msg.y))
					{
						controls[i]->OnMouseUp(msg.x, msg.y);
						break;
					}
				}
			}
			else if (msg.message == WM_MOUSEMOVE)
			{
				mouseDown = msg.lbutton;

				if (focusIndex != -1) controls[focusIndex]->OnMouseMoveFocused(msg.x, msg.y);

				//for (Control* control : controls)
				for (int i = controls.size() - 1; i; --i)
				{
					Control* control = controls[i];
					bool flag = false;
					if (control->Check(msg.x, msg.y))
					{
						control->OnMouseMove(msg.x, msg.y);
						flag = true;
						break;
					}
					if (!flag)
					{
						SetCursor(hWnd, IDC_ARROW);
					}
				}
			}
			else if (msg.message == WM_CHAR)
			{
				if (focusIndex != -1)
				{
					controls[focusIndex]->OnChar(msg.ch);
				}
			}
			else if (msg.message == WM_KEYDOWN)
			{
				if (msg.vkcode == VK_CONTROL) ctrl = true;
				if (msg.vkcode == VK_SHIFT) shift = true;

				if (focusIndex != -1)
				{
					controls[focusIndex]->OnKeyDown(ctrl, shift, msg.vkcode);
					//controls[focusIndex]->OnKeyDown(GetKeyState(VK_CONTROL), GetKeyState(VK_SHIFT), msg.vkcode);
				}

				if (msg.vkcode == VK_TAB && !ctrl && !mouseDown) OnTabKey(shift);
			}
			else if (msg.message == WM_KEYUP)
			{
				if (msg.vkcode == VK_CONTROL) ctrl = false;
				if (msg.vkcode == VK_SHIFT) shift = false;

				if (focusIndex != -1)
				{
					controls[focusIndex]->OnKeyUp(ctrl, shift, msg.vkcode);
				}
			}
			peekmessage(NULL, EM_MOUSE | EM_KEY | EM_CHAR);
			FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		}

		//if (!mouseOnWindow) mouseDown = false;

		/// 处理焦点
		int lFocusIndex = focusIndex;
		for (Control* control : controls)
		{
			/// 重置控件的焦点状态
			control->focused = false;
		}
		if (focusIndex >= int(controls.size())) focusIndex = -1;
		if (controls.size() != 0 && focusIndex != -1)
		{
			/// 将焦点控件设为焦点
			controls[focusIndex]->focused = true;
		}

		/// 更新控件
		for (Control* control : controls)
		{
			control->Update();
		}

		/// 执行用户自定义函数
		if (onUpdateFunction != nullptr) onUpdateFunction();
	}

	void Form::Render()
	{
		/// 填充窗口背景
		setbkcolor(0xeeeeee);
		cleardevice();

		/// 渲染控件
		for (Control* control : controls)
		{
			control->Render();
		}

		/// 执行绘制任务
		FlushBatchDraw();
	}

	void Form::AddControl(Control& controlRef)
	{
		controls.push_back(&controlRef);
	}

	void Form::SetFocus(int focusIndex)
	{
		this->focusIndex = focusIndex;
	}

	void Form::OnTabKey(bool shift)
	{
		/// Tab或Shift+Tab不同方向切换焦点
		do
		{
			if (shift) (--focusIndex += controls.size()) %= controls.size();
			else (++focusIndex) %= controls.size();
		} while (!controls[focusIndex]->focusable);
	}

	void Form::SetFocus(Control* focusControl)
	{
		for (int i = 0; i < int(controls.size()); ++i)
		{
			if (controls[i] == focusControl)
			{
				SetFocus(i);
				break;
			}
		}
	}

	void Form::RemoveControl(Control& controlRef)
	{
		if (controlRef.focused) SetFocus(-1);
		for (int i = 0; i < int(controls.size()); ++i)
		{
			if (controls[i] == &controlRef)
			{
				controls.erase(controls.begin() + i);
			}
		}
	}

	Label::Label(Form& form, int left, int top, wstring text, LOGFONT* font) :
		Control(form, left, top, 0, 0, false, nullptr, nullptr), text(text), font(font)
	{
	}

	void Label::Render()
	{
		/// 计算行高
		settextstyle(font);
		lineHeight = textheight(L"|") + lineSpacing;

		/// 绘制文字
		settextstyle(font);
		settextcolor(BLACK);
		setbkcolor(WHITE);
		setbkmode(TRANSPARENT);
		vector<wstring> splitText = MyString::split(text, L'\n');
		for (int i = 0; i < int(splitText.size()); ++i)
		{
			outtextxy(left, top + lineHeight * i, splitText[i].c_str()); /// 按行打印文字
		}
	}

	TextBox::TextBox(Form& form, int left, int top, int width, int height, LOGFONT* font, bool readOnly, bool multiLine, void (*onUpdateFunction)(), void (*onClickFunction)()) :
		Control(form, left, top, width, height, true, onUpdateFunction, onClickFunction), font(font), readOnly(readOnly), multiLine(multiLine)
	{
		textLeft = left + 5;
		textTop = top + 5;
	}

	void TextBox::Update()
	{
		/// 将L'\r'都替换为L'\n'，方便渲染
		wstring::size_type startPos = 0;
		while (startPos != wstring::npos)
		{
			startPos = text.find(L'\r');
			if (startPos != wstring::npos) /// wstring::npos表示没有找到该字符  
			{
				text.replace(startPos, 1, L"\n");
			}
		}

		if (!focused) selStart = curPos = 0;

		/// 调用基类方法
		Control::Update();
	}

	void TextBox::Render()
	{
		/// 绘制文本框形状
		setlinecolor(BLACK);
		setlinestyle(PS_SOLID);
		setfillcolor(readOnly ? 0xdddddd : WHITE);
		fillrectangle(left, top, left + width, top + height);

		/// 计算行高
		settextstyle(font);
		lineHeight = textheight(L"|") + lineSpacing;

		if (focused)
		{
			/// 绘制选择区间
			int selPosL, selPosR;
			selPosL = min(selStart, curPos);
			selPosR = max(selStart, curPos);
			setfillcolor(readOnly ? 0xb1b1b1 : 0xcccccc);
			setlinestyle(PS_NULL);
			//vector<wstring> splitText1 = MyString::split(text.substr(0, selPosL), L'\n'); /// 按换行符分隔选中区域左侧之前的文字
			vector<wstring> splitText2 = MyString::split(text.substr(0, selPosR), L'\n'); /// 按换行符分隔选中区域右侧之前的文字
			int l = selPosL;
			for (int i = 0; i < int(splitText2.size()); l -= splitText2[i++].size() + 1)
			{
				if (l > int(splitText2[i].size())) continue;
				if (l < 0) l = 0;
				int curTop = top + 3 + lineHeight * i;
				fillrectangle(textLeft + textwidth(splitText2[i].substr(0, l).c_str()), curTop, textLeft + textwidth(splitText2[i].substr(0).c_str()), curTop + lineHeight); /// 按行填充矩形
			}
		}

		/// 绘制文字
		settextstyle(font);
		settextcolor(BLACK);
		setbkcolor(WHITE);
		setbkmode(TRANSPARENT);
		vector<wstring> splitText = MyString::split(text, L'\n'); /// 按换行符分隔文字 //TODO 可以动态维护这个东西，不然重复太多次了，但是现在在赶开发进度，这种简洁性优化可有可无了
		for (int i = 0; i < int(splitText.size()); ++i)
		{
			outtextxy(textLeft, textTop + lineHeight * i, splitText[i].c_str()); /// 按行打印文字
		}

		if (focused)
		{
			/*/// 绘制选择区间
			int selPosL, selPosR;
			selPosL = min(selStart, curPos);
			selPosR = max(selStart, curPos);
			setbkcolor(0xcccccc);
			setbkmode(OPAQUE);
			//outtextxy(textLeft + textwidth(text.substr(0, selPosL).c_str()), textTop, text.substr(selPosL, selPosR - selPosL).c_str());*/
			
			/*
			int selPosL, selPosR;
			selPosL = min(selStart, curPos);
			selPosR = max(selStart, curPos);
			setfillcolor(0xcccccc);
			//fillrectangle(textLeft + textwidth(text.substr(0, selPosL).c_str()), top + 3, textwidth(text.substr(0, selPosR).c_str()), top + height - 3);
			*/

			/// 绘制光标
			vector<wstring> splitSubText = MyString::split(text.substr(0, curPos), L'\n'); /// 按换行符分隔光标前的文字
			int cursorLeft = textLeft + textwidth(splitSubText[splitSubText.size() - 1].c_str());
			int cursorTop = top + 3 + lineHeight * (splitSubText.size() - 1);
			setlinecolor(BLACK);
			setlinestyle(PS_SOLID);
			settextstyle(font);
			line(cursorLeft, cursorTop, cursorLeft, cursorTop + textheight(L"|") + 3);
		}

		/// 调用基类方法
		Control::Render();
	}

	void TextBox::OnMouseDown(int x, int y)
	{
		/// 确定闪烁光标的位置
		settextstyle(font);
		OnMouseMoveFocused(x, y);
		selStart = curPos;

		/// 调用基类方法
		Control::OnMouseDown(x, y);
	}

	void TextBox::OnMouseUp(int x, int y)
	{
		/// 调用基类方法
		Control::OnMouseUp(x, y);
	}

	void TextBox::OnMouseMove(int x, int y)
	{
		/// 鼠标光标样式设为I字形
		SetCursor(form.hWnd, IDC_IBEAM);

		/// 调用基类方法
		Control::OnMouseMove(x, y);
	}

	void TextBox::OnMouseMoveFocused(int x, int y)
	{
		/// 处理拖动选择或点击
		if (form.mouseDown)
		{
			int curLine = (y - textTop) / lineHeight; /// 根据位置直接计算当前行行号（从0开始计，可能越界）
			vector<wstring> splitText = MyString::split(text, L'\n'); /// 按换行符分隔文字

			/// 处理越界
			if (curLine < 0) curLine = 0;
			if (curLine >= int(splitText.size())) curLine = splitText.size() - 1;

			/// 先将当前光标位置放在当前行行首
			curPos = 0;
			for (int i = 0; i < curLine; ++i)
			{
				curPos += splitText[i].size() + 1;
			}

			int relPos = 0; /// 光标在当前行的位置

			/// 求relPos
			for (;
				relPos < int(splitText[curLine].size()) &&
				x - textLeft >= textwidth(splitText[curLine].substr(0, relPos + 1).c_str()) - textwidth(text[relPos]) / 2;
				++relPos);

			/// 得到curPos最终值
			curPos += relPos;
		}

		/// 调用基类方法
		Control::OnMouseMoveFocused(x, y);
	}

	void TextBox::OnChar(wchar_t c)
	{
		if (readOnly) return;

		//if (form.mouseDown || form.ctrl) return;
		if (form.ctrl) return;

		//MessageBoxW(form.hWnd, to_wstring(c).c_str(), L"KeyDown", 0);

		int selPosL = min(selStart, curPos), selPosR = max(selStart, curPos);
		switch (c)
		{
		case L'\b':
			if (selPosL == selPosR)
			{
				if (curPos > 0)
				{
					text = text.substr(0, curPos - 1) + text.substr(curPos);
					selStart = --curPos;
				}
			}
			else
			{
				text = text.substr(0, selPosL) + text.substr(selPosR);
				selStart = curPos = selPosL;
			}
			break;
		/*case L'\r':
		case L'\n':
			if (!multiLine) form.ResetFocus();
			break;*/
		default:
			//if (c >= L'0' && c <= L'9' || c >= L'A' && c <= L'Z' || c >= L'a' && c <= L'z')
			/*if (c >= 32 && c <= 126 || true)
			{
				text = text.substr(0, selPosL) + c + text.substr(selPosR);
				selStart = curPos = selPosL + 1;
				//if (selPosL == selPosR) selStart = ++curPos;
				//else selStart = curPos = selPosL + 1;
			}*/
			if ((c == L'\r' || c == L'\n') && !multiLine)
			{
				if (defButton == nullptr) form.OnTabKey(false);//form.SetFocus(-1);
				else
				{
					form.SetFocus(defButton);
					defButton->OnKeyDown(false, false, VK_RETURN);
				}
			}
			else if (c != '\t')// if (c >= 32 && c <= 126)
			{
				text = text.substr(0, selPosL) + c + text.substr(selPosR);
				selStart = curPos = selPosL + 1;
			}
			break;
		}

		/// 调用基类方法
		Control::OnChar(c);
	}

	void TextBox::OnKeyDown(bool ctrl, bool shift, BYTE vkcode)
	{
		//if (form.mouseDown) return;

		//MessageBoxW(form.hWnd, ((wstring)(ctrl ? L"[Ctrl]" : L"") + (wstring)(shift ? L"[Shift]" : L"") + to_wstring(vkcode)).c_str(), L"KeyDown", 0);

		switch (vkcode)
		{
		case VK_DELETE:
			if (!readOnly)
			{
				int selPosL = min(selStart, curPos), selPosR = max(selStart, curPos);
				if (selPosL == selPosR)
				{
					if (curPos < int(text.length()))
					{
						text = text.substr(0, curPos) + text.substr(curPos + 1);
					}
				}
				else
				{
					text = text.substr(0, selPosL) + text.substr(selPosR);
					selStart = curPos = selPosL;
				}
			}
			break;
		case VK_HOME:
			if (ctrl) curPos = 0;
			else
			{
				int lineHome = text.find_last_of(L'\n', curPos);
				if (lineHome != -1) curPos = lineHome + 1;
				else curPos = 0;
			}
			if (!shift) selStart = curPos;
			break;
		case VK_END:
			if (ctrl) curPos = text.length();
			else
			{
				int lineEnd = text.find(L'\n', curPos);
				if (lineEnd != -1) curPos = lineEnd;
				else curPos = text.length();
			}
			if (!shift) selStart = curPos;
			break;
		case 'A':
			if (ctrl)
			{
				selStart = 0;
				curPos = text.length();
			}
			break;
		case 'C':
		case 'X':
			if (ctrl)
			{
				while (!OpenClipboard(form.hWnd));

				int selPosL = min(selStart, curPos), selPosR = max(selStart, curPos);
				wstring copyStr = text.substr(selPosL, selPosR - selPosL);
				int nCount = copyStr.size();
				HGLOBAL hGlobalMem = nullptr;
				if (nCount != 0) hGlobalMem = GlobalAlloc(GMEM_MOVEABLE, (nCount + 1) * sizeof(TCHAR));
				if (hGlobalMem != nullptr)
				{
					LPTSTR lpszStr = (LPTSTR)GlobalLock(hGlobalMem);
					if (lpszStr != nullptr)
					{
						memcpy(lpszStr, copyStr.c_str(), nCount * sizeof(TCHAR));
						lpszStr[nCount] = (TCHAR)0;
						EmptyClipboard();
						SetClipboardData(CF_UNICODETEXT, hGlobalMem);
						if (vkcode == 'X' && !readOnly)
						{
							text = text.substr(0, selPosL) + text.substr(selPosR);
							selStart = curPos = selPosL;
						}
					}
					GlobalUnlock(hGlobalMem);
				}

				CloseClipboard();

				//else MessageBox(form.hWnd, L"Failed to open clipboard!", L"Copy", 0);
			}
			break;
		case 'V':
			if (!readOnly && ctrl)
			{
				while (!OpenClipboard(form.hWnd));

				if (IsClipboardFormatAvailable(CF_UNICODETEXT))
				{
					HANDLE hClip;
					wchar_t* pBuf;
					hClip = GetClipboardData(CF_UNICODETEXT);
					pBuf = (wchar_t*)GlobalLock(hClip);
					if (pBuf != nullptr) {
						wstring tt(pBuf);
						int selPosL = min(selStart, curPos), selPosR = max(selStart, curPos);
						text = text.substr(0, selPosL) + tt + text.substr(selPosR);
						selStart = curPos = selPosL + tt.length();
					}
					GlobalUnlock(hClip);
				}

				CloseClipboard();

				//else MessageBox(form.hWnd, L"Failed to paste from clipboard!", L"Paste", 0);
			}
			break;
		case VK_LEFT: /// Left arrow key
			--curPos;
			if (!shift) selStart = curPos;
			if (selStart < 0) selStart = 0;
			if (curPos < 0) curPos = 0;
			break;
		case VK_RIGHT: /// Right arrow key
			++curPos;
			if (!shift) selStart = curPos;
			if (selStart > int(text.length())) selStart = text.length();
			if (curPos > int(text.length())) curPos = text.length();
			break;
		case VK_UP: /// Up arrow key
		{
			vector<wstring> splitText = MyString::split(text, L'\n'); /// 按换行符分隔文字
			int l = 0;
			for (int i = 0; i < int(splitText.size()); l += splitText[i++].size() + 1)
			{
				if (curPos - l <= int(splitText[i].size()))
				{
					if (i == 0) curPos = 0;
					else
					{
						curPos = l - 1 - splitText[i - 1].size() + min(curPos - l, int(splitText[i - 1].size()));
					}
					break;
				}
			}
			if (!shift) selStart = curPos;
			if (selStart < 0) selStart = 0;
			if (curPos < 0) curPos = 0;
			break;
		}
		case VK_DOWN: /// Down arrow key
		{
			vector<wstring> splitText = MyString::split(text, L'\n'); /// 按换行符分隔文字
			int l = 0;
			for (int i = 0; i < int(splitText.size()); l += splitText[i++].size() + 1)
			{
				if (curPos - l <= int(splitText[i].size()))
				{
					if (i == splitText.size() - 1) curPos = text.length();
					else
					{
						curPos = l + 1 + splitText[i].size() + min(curPos - l, int(splitText[i + 1].size()));
					}
					break;
				}
			}
			if (!shift) selStart = curPos;
			if (selStart > int(text.length())) selStart = text.length();
			if (curPos > int(text.length())) curPos = text.length();
			break;
		}
		}

		/// 调用基类方法
		Control::OnKeyDown(ctrl, shift, vkcode);
	}

	Button::Button(Form& form, int left, int top, int width, int height, LOGFONT* font, wstring text, void (*onUpdateFunction)(), void (*onClickFunction)()):
		Control(form, left, top, width, height, true, onUpdateFunction, onClickFunction), font(font), text(text)
	{
	}

	void Button::Update()
	{
		if (!form.mouseDown) mouseDown = false;
		if (--anim < 0) anim = 0;

		/// 调用基类方法
		Control::Update();
	}

	void Button::Render()
	{
		/// 绘制按钮形状
		setlinecolor(BLACK);
		setlinestyle(PS_SOLID);
		setfillcolor(0xd8d8d8);
		fillrectangle(left, top, left + width, top + height);

		if (focused)
		{
			setlinecolor(BLACK);
			setlinestyle(PS_DASH);
			fillrectangle(left + 2, top + 2, left + width - 2, top + height - 2);

			if (mouseDown || anim > 0)
			{
				setlinestyle(PS_SOLID);
				int rgb = 0xd8 - (mouseDown ? (0xd8 - 0xc8) : int(float(anim) / animTime * (0xd8 - 0xc8)));
				int fillColor = RGB(rgb, rgb, rgb);
				setfillcolor(fillColor);
				fillrectangle(left, top, left + width, top + height);
			}
		}

		/// 绘制文字
		settextstyle(font);
		settextcolor(BLACK);
		setbkcolor(WHITE);
		setbkmode(TRANSPARENT);
		outtextxy(left + width / 2 - textwidth(text.c_str()) / 2, top + height / 2 - textheight(text.c_str()) / 2, text.c_str());

		/// 调用基类方法
		Control::Render();
	}

	void Button::OnMouseDown(int x, int y)
	{
		OnMouseMoveFocused(x, y);

		/// 调用基类方法
		Control::OnMouseDown(x, y);
	}

	void Button::OnMouseUp(int x, int y)
	{
		//if (mouseDown) anim = animTime;
		mouseDown = false;

		/// 调用基类方法
		Control::OnMouseUp(x, y);
	}

	void Button::OnMouseMoveFocused(int x, int y)
	{
		if (form.mouseDown) mouseDown = true;

		/// 调用基类方法
		Control::OnMouseMoveFocused(x, y);
	}

	void Button::OnKeyDown(bool ctrl, bool shift, BYTE vkcode)
	{
		switch (vkcode)
		{
		case VK_RETURN:
			if (!ctrl && !shift)
			{
				anim = animTime;
				OnMouseUp(left + 1, top + 1); /// 回车，模拟鼠标点击
			}
			break;
		}

		/// 调用基类方法
		Control::OnKeyDown(ctrl, shift, vkcode);
	}

	void Button::OnKeyUp(bool ctrl, bool shift, BYTE vkcode)
	{
		switch (vkcode)
		{
		case VK_RETURN:
			//OnMouseUp(left + 1, top + 1);
			break;
		}

		/// 调用基类方法
		Control::OnKeyUp(ctrl, shift, vkcode);
	}
}