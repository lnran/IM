// MyButton.cpp: 实现文件
//

#include "stdafx.h"
#include "QQ.h"
#include "MyButton.h"


// MyButton

IMPLEMENT_DYNAMIC(MyButton, CMFCButton)

MyButton::MyButton()
{

}

MyButton::~MyButton()
{
}


void MyButton::SetButtonStyle()
{
	m_nFlatStyle = BUTTONSTYLE_NOBORDERS;
	m_bDrawFocus = FALSE;
}

BEGIN_MESSAGE_MAP(MyButton, CMFCButton)
END_MESSAGE_MAP()



// MyButton 消息处理程序


