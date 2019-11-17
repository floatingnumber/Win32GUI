#include "EditText.h"

EditText::EditText()
{
}

EditText::EditText(Control* parent, std::string name, int width, int height)
	: TextControl(name, width, height)
{
	mXMargin = 0; mYMargin = 6;
	mStyle |= WS_CHILD | WS_VISIBLE;
	mParent = parent;
	mType = WC_EDIT;
	create();
	autoSize();
}

void EditText::replaceSelection(std::string str)
{
	if (mCreated)
		SendMessage(mHwnd, EM_REPLACESEL, TRUE, (LPARAM)str.c_str());
}

void EditText::setOnCharLimit(f_onCharLimit call)
{
	mOnCharLimit = call;
}

void EditText::setCharLimit(int val)
{
	mCharLimit = val;
	if (mCreated)
		SendMessage(mHwnd, EM_SETLIMITTEXT, mCharLimit, NULL);
}

void EditText::setSelection(int start, int end)
{
	// Does not draw blue
	if (mCreated)
		SendMessage(mHwnd, EM_SETSEL, start, end);
}

void EditText::setNumbersOnly(bool state)
{
	if (state)
		appendFlag(ES_NUMBER);
	else
		removeFlag(ES_NUMBER);
}

void EditText::setReadOnly(bool state)
{
	if (state)
		appendFlag(ES_READONLY);
	else
		removeFlag(ES_READONLY);

	// ES_READONLY needs special message
	// when control is already created.
	if (mCreated)
		SendMessage(mHwnd, EM_SETREADONLY, state, NULL);
}

void EditText::setPassword(bool state)
{
	if (state)
		appendFlag(ES_PASSWORD);
	else
		removeFlag(ES_PASSWORD);

	// ES_PASSWORD needs special message
	// when control is already created.
	if (mCreated)
		SendMessage(mHwnd, EM_SETPASSWORDCHAR, state ? '*' : FALSE, NULL);
}

void EditText::setTextCase(TextCase state)
{
	removeFlag(ES_UPPERCASE);
	removeFlag(ES_LOWERCASE);

	if (state == TextCase::UpperCase)
		appendFlag(ES_UPPERCASE);
	if (state == TextCase::LowerCase)
		appendFlag(ES_LOWERCASE);
}

std::string EditText::getSelection()
{
	if (!mCreated)
		return "";
	DWORD start, end;
	SendMessage(mHwnd, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
	return mText.substr(start, end - start);
}

void EditText::setMargin(Margin margin)
{
	removeFlag(ES_LEFT);
	removeFlag(ES_CENTER);
	removeFlag(ES_RIGHT);

	if (margin == Margin::Left)
		appendFlag(ES_LEFT);
	if (margin == Margin::Center)
		appendFlag(ES_CENTER);
	if (margin == Margin::Right)
		appendFlag(ES_RIGHT);
}

Margin EditText::getMargin()
{
	if (hasFlag(ES_LEFT))
		return Margin::Left;
	if (hasFlag(ES_CENTER))
		return Margin::Center;
	if (hasFlag(ES_RIGHT))
		return Margin::Right;

	return Margin::Undefined;
}

EditText::EditText(std::string name, int width, int height)
	: TextControl(name, width, height)
{
	mXMargin = 0; mYMargin = 6;
	mStyle |= WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
	mType = WC_EDIT;
}

LRESULT EditText::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case EN_CHANGE: /* Text Change, try autosize */
		// We must pull text here for the autosize to work
		mText = pullWindowText();
		autoSize();
		break;
	case EN_MAXTEXT:
		if (mOnCharLimit)
			mOnCharLimit(this);
		break;
	case EM_SETSEL:
		std::cout << "xd" << std::endl;
	}
	return true;
}