#pragma once
#include <graphics.h>
#include <string>
#include <vector>
using namespace std;

namespace Interface
{
	class Form;
	class Button;

	class Control
	{
	private:
		void (*onClickFunction)();
		void (*onUpdateFunction)();

	public:
		Form& form;
		int left, top, width, height;
		bool focused = false;
		bool focusable;

		Control(Form& form, int left, int top, int width, int height, bool focusable, void (*onUpdateFunction)(), void (*onClickFunction)());

		bool Check(int x, int y);

		virtual void Update();

		virtual void Render();

		virtual void OnMouseDown(int x, int y);

		virtual void OnMouseUp(int x, int y);

		virtual void OnMouseMove(int x, int y);

		virtual void OnMouseMoveFocused(int x, int y);

		virtual void OnChar(wchar_t c);

		virtual void OnKeyDown(bool ctrl, bool shift, BYTE vkcode);

		virtual void OnKeyUp(bool ctrl, bool shift, BYTE vkcode);
	};

	class Form
	{
	private:
		void (*onUpdateFunction)();
		int width, height;
		wstring caption;
		vector<Control*> controls;
		int focusIndex = -1;

	public:
		HWND hWnd = nullptr;

		bool mouseDown = false, ctrl = false, shift = false;

		Form(int width, int height, wstring caption, void (*onUpdateFunction)() = nullptr);

		void MsgBox(wstring text, wstring caption, UINT type = 0U)
		{
			if (hWnd == nullptr) return;
			MessageBox(hWnd, text.c_str(), caption.c_str(), type);
		}

		void Main();

		void Update();

		void Render();

		void AddControl(Control& controlRef);

		void RemoveControl(Control& controlRef);

		void SetFocus(int focusIndex);

		void SetFocus(Control* focusControl);

		void OnTabKey(bool shift);
	};

	class Label : public Control
	{
	private:
		const int lineSpacing = 3;
		int lineHeight = 0;
		LOGFONT* font;

	public:
		wstring text = L"";

		Label(Form& form, int left, int top, wstring text, LOGFONT* font = nullptr);

		//void Update();

		void Render();
	};

	class TextBox : public Control
	{
	private:
		const int lineSpacing = 3;
		int textLeft, textTop;
		LOGFONT* font;
		bool readOnly;
		bool multiLine;
		int curPos = 0, selStart = 0;
		int lineHeight = 0;

	public:
		wstring text = L"";

		Button* defButton = nullptr;

		TextBox(Form& form, int left, int top, int width, int height, LOGFONT* font = nullptr, bool readOnly = false, bool multiLine = false, void (*onUpdateFunction)() = nullptr, void (*onClickFunction)() = nullptr);

		void Update();

		void Render();

		void OnMouseDown(int x, int y);

		void OnMouseUp(int x, int y);

		void OnMouseMove(int x, int y);

		void OnMouseMoveFocused(int x, int y);

		void OnChar(wchar_t c);

		void OnKeyDown(bool ctrl, bool shift, BYTE vkcode);

		//void OnKeyUp(bool ctrl, bool shift, BYTE vkcode);
	};

	class Button : public Control
	{
	private:
		wstring text;
		LOGFONT* font;
		bool mouseDown = false;
		const int animTime = 60;
		int anim = 0;

	public:
		Button(Form& form, int left, int top, int width, int height, LOGFONT* font = nullptr, wstring text = L"", void (*onUpdateFunction)() = nullptr, void (*onClickFunction)() = nullptr);

		void Update();

		void Render();

		void OnMouseDown(int x, int y);

		void OnMouseUp(int x, int y);

		//void OnMouseMove(int x, int y);

		void OnMouseMoveFocused(int x, int y);

		//void OnChar(wchar_t c);

		void OnKeyDown(bool ctrl, bool shift, BYTE vkcode);

		void OnKeyUp(bool ctrl, bool shift, BYTE vkcode);
	};
}