/*
 * =====================================================================================
 *
 *       Filename:  ViewWidget.h
 *        Created:  2014/7/30 11:21:10
 *       Compiler:  vs2010 
 *         Author:  jingtao.wu
 *
 *    Description:  参数显示窗体
 *
 * =====================================================================================
 */

#ifndef  VIEWWIDGET_INC
#define  VIEWWIDGET_INC

#include <qwidget.h>
#include "ui_view.h"
//全局参数
class GlobalArgs
{
public:
	static int g_radio;
	static int g_frame;
};

class ViewWidget :
	public QWidget
{
public:
	ViewWidget(QWidget *parent = NULL);
	~ViewWidget(void);

public:
	//设置保存提示文本
	void SetSave(bool save);

//继承
protected:
	//每秒显示数据值
 void timerEvent( QTimerEvent *qevent );

private:
	Ui::view mViewUi;
	//是否保存了数据
	bool mbSave;
};

#endif   /* ----- #ifndef VIEWWIDGET_INC  ----- */
