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
		/// ִ���û��Զ�������¼�
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
		/// ִ���û��Զ������¼�
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
		/// ��ʼ������
		hWnd = initgraph(width, height);
		
		/// ���ڱ���
		SetWindowText(hWnd, caption.c_str());

		BeginBatchDraw();
		
		/// ��ѭ��
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
		/// ��������
		ExMessage msg;
		bool mouseOnWindow = false; /// ����Ƿ��ڴ�����
		while (peekmessage(&msg, EM_MOUSE | EM_KEY | EM_CHAR, false))
		{
			if (msg.message == WM_LBUTTONDOWN)
			{
				mouseDown = true;
				ctrl = shift = false; /// ��Ϊ�������ⲻ�ý��������ctrl��shift

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

		/// ������
		int lFocusIndex = focusIndex;
		for (Control* control : controls)
		{
			/// ���ÿؼ��Ľ���״̬
			control->focused = false;
		}
		if (focusIndex >= int(controls.size())) focusIndex = -1;
		if (controls.size() != 0 && focusIndex != -1)
		{
			/// ������ؼ���Ϊ����
			controls[focusIndex]->focused = true;
		}

		/// ���¿ؼ�
		for (Control* control : controls)
		{
			control->Update();
		}

		/// ִ���û��Զ��庯��
		if (onUpdateFunction != nullptr) onUpdateFunction();
	}

	void Form::Render()
	{
		/// ��䴰�ڱ���
		setbkcolor(0xeeeeee);
		cleardevice();

		/// ��Ⱦ�ؼ�
		for (Control* control : controls)
		{
			control->Render();
		}

		/// ִ�л�������
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
		/// Tab��Shift+Tab��ͬ�����л�����
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
		/// �����и�
		settextstyle(font);
		lineHeight = textheight(L"|") + lineSpacing;

		/// ��������
		settextstyle(font);
		settextcolor(BLACK);
		setbkcolor(WHITE);
		setbkmode(TRANSPARENT);
		vector<wstring> splitText = MyString::split(text, L'\n');
		for (int i = 0; i < int(splitText.size()); ++i)
		{
			outtextxy(left, top + lineHeight * i, splitText[i].c_str()); /// ���д�ӡ����
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
		/// ��L'\r'���滻ΪL'\n'��������Ⱦ
		wstring::size_type startPos = 0;
		while (startPos != wstring::npos)
		{
			startPos = text.find(L'\r');
			if (startPos != wstring::npos) /// wstring::npos��ʾû���ҵ����ַ�  
			{
				text.replace(startPos, 1, L"\n");
			}
		}

		if (!focused) selStart = curPos = 0;

		/// ���û��෽��
		Control::Update();
	}

	void TextBox::Render()
	{
		/// �����ı�����״
		setlinecolor(BLACK);
		setlinestyle(PS_SOLID);
		setfillcolor(readOnly ? 0xdddddd : WHITE);
		fillrectangle(left, top, left + width, top + height);

		/// �����и�
		settextstyle(font);
		lineHeight = textheight(L"|") + lineSpacing;

		if (focused)
		{
			/// ����ѡ������
			int selPosL, selPosR;
			selPosL = min(selStart, curPos);
			selPosR = max(selStart, curPos);
			setfillcolor(readOnly ? 0xb1b1b1 : 0xcccccc);
			setlinestyle(PS_NULL);
			//vector<wstring> splitText1 = MyString::split(text.substr(0, selPosL), L'\n'); /// �����з��ָ�ѡ���������֮ǰ������
			vector<wstring> splitText2 = MyString::split(text.substr(0, selPosR), L'\n'); /// �����з��ָ�ѡ�������Ҳ�֮ǰ������
			int l = selPosL;
			for (int i = 0; i < int(splitText2.size()); l -= splitText2[i++].size() + 1)
			{
				if (l > int(splitText2[i].size())) continue;
				if (l < 0) l = 0;
				int curTop = top + 3 + lineHeight * i;
				fillrectangle(textLeft + textwidth(splitText2[i].substr(0, l).c_str()), curTop, textLeft + textwidth(splitText2[i].substr(0).c_str()), curTop + lineHeight); /// ����������
			}
		}

		/// ��������
		settextstyle(font);
		settextcolor(BLACK);
		setbkcolor(WHITE);
		setbkmode(TRANSPARENT);
		vector<wstring> splitText = MyString::split(text, L'\n'); /// �����з��ָ����� //TODO ���Զ�̬ά�������������Ȼ�ظ�̫����ˣ����������ڸϿ������ȣ����ּ�����Ż����п�����
		for (int i = 0; i < int(splitText.size()); ++i)
		{
			outtextxy(textLeft, textTop + lineHeight * i, splitText[i].c_str()); /// ���д�ӡ����
		}

		if (focused)
		{
			/*/// ����ѡ������
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

			/// ���ƹ��
			vector<wstring> splitSubText = MyString::split(text.substr(0, curPos), L'\n'); /// �����з��ָ����ǰ������
			int cursorLeft = textLeft + textwidth(splitSubText[splitSubText.size() - 1].c_str());
			int cursorTop = top + 3 + lineHeight * (splitSubText.size() - 1);
			setlinecolor(BLACK);
			setlinestyle(PS_SOLID);
			settextstyle(font);
			line(cursorLeft, cursorTop, cursorLeft, cursorTop + textheight(L"|") + 3);
		}

		/// ���û��෽��
		Control::Render();
	}

	void TextBox::OnMouseDown(int x, int y)
	{
		/// ȷ����˸����λ��
		settextstyle(font);
		OnMouseMoveFocused(x, y);
		selStart = curPos;

		/// ���û��෽��
		Control::OnMouseDown(x, y);
	}

	void TextBox::OnMouseUp(int x, int y)
	{
		/// ���û��෽��
		Control::OnMouseUp(x, y);
	}

	void TextBox::OnMouseMove(int x, int y)
	{
		/// �������ʽ��ΪI����
		SetCursor(form.hWnd, IDC_IBEAM);

		/// ���û��෽��
		Control::OnMouseMove(x, y);
	}

	void TextBox::OnMouseMoveFocused(int x, int y)
	{
		/// �����϶�ѡ�����
		if (form.mouseDown)
		{
			int curLine = (y - textTop) / lineHeight; /// ����λ��ֱ�Ӽ��㵱ǰ���кţ���0��ʼ�ƣ�����Խ�磩
			vector<wstring> splitText = MyString::split(text, L'\n'); /// �����з��ָ�����

			/// ����Խ��
			if (curLine < 0) curLine = 0;
			if (curLine >= int(splitText.size())) curLine = splitText.size() - 1;

			/// �Ƚ���ǰ���λ�÷��ڵ�ǰ������
			curPos = 0;
			for (int i = 0; i < curLine; ++i)
			{
				curPos += splitText[i].size() + 1;
			}

			int relPos = 0; /// ����ڵ�ǰ�е�λ��

			/// ��relPos
			for (;
				relPos < int(splitText[curLine].size()) &&
				x - textLeft >= textwidth(splitText[curLine].substr(0, relPos + 1).c_str()) - textwidth(text[relPos]) / 2;
				++relPos);

			/// �õ�curPos����ֵ
			curPos += relPos;
		}

		/// ���û��෽��
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

		/// ���û��෽��
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
			vector<wstring> splitText = MyString::split(text, L'\n'); /// �����з��ָ�����
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
			vector<wstring> splitText = MyString::split(text, L'\n'); /// �����з��ָ�����
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

		/// ���û��෽��
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

		/// ���û��෽��
		Control::Update();
	}

	void Button::Render()
	{
		/// ���ư�ť��״
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

		/// ��������
		settextstyle(font);
		settextcolor(BLACK);
		setbkcolor(WHITE);
		setbkmode(TRANSPARENT);
		outtextxy(left + width / 2 - textwidth(text.c_str()) / 2, top + height / 2 - textheight(text.c_str()) / 2, text.c_str());

		/// ���û��෽��
		Control::Render();
	}

	void Button::OnMouseDown(int x, int y)
	{
		OnMouseMoveFocused(x, y);

		/// ���û��෽��
		Control::OnMouseDown(x, y);
	}

	void Button::OnMouseUp(int x, int y)
	{
		//if (mouseDown) anim = animTime;
		mouseDown = false;

		/// ���û��෽��
		Control::OnMouseUp(x, y);
	}

	void Button::OnMouseMoveFocused(int x, int y)
	{
		if (form.mouseDown) mouseDown = true;

		/// ���û��෽��
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
				OnMouseUp(left + 1, top + 1); /// �س���ģ�������
			}
			break;
		}

		/// ���û��෽��
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

		/// ���û��෽��
		Control::OnKeyUp(ctrl, shift, vkcode);
	}
}