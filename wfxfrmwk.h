// This is a part of the Widget Foundation Classes.
// Copyright (C) Grant Ward (grant.ward@gmail.com)
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Widget Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Widget Foundation Classes product.
//

#pragma once

#include "wfx.h"

BEGIN_NAMESPACE_WFX

class MsgMap;
class Widget;
class Document;
class View;

class WFX_API View : public Widget
{
	WFX_DECLARE_FACTORY(View);
protected:
	View();
public:
	Document* GetDocument() const;
	void SetDocument(Document* pDoc);
protected:
	Document* m_pDoc;
};
typedef Factory<View>							ViewFactory;
typedef SharedPtr<ViewFactory>					PViewFactory;
typedef const PViewFactory						CPViewFactroy;
typedef SharedPtr<View>							PView;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Document : public MsgMap
{
	WFX_DECLARE_FACTORY(Document);
protected:
	Document();
public:
	virtual BOOL ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID);
public:
	virtual BOOL Open();
	virtual BOOL Close();
	View* GetView() const;
	void SetView(View* pView);
protected:
	View* m_pView;
};
typedef Factory<Document>						DocFactory;
typedef SharedPtr<DocFactory>					PDocFactory;
typedef const PDocFactory						CPDocFactory;
typedef SharedPtr<Document>						PDocutment;
typedef std::pair<PDocFactory, PViewFactory>	Doc2ViewFactory;
typedef std::vector<Doc2ViewFactory>			Doc2ViewFactoryList;
typedef std::pair<PDocutment, PView>			Doc2View;
typedef SharedPtr<Doc2View>						PDoc2View;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API DocManager : public MsgMap
{
public:
	DocManager();
public:
	virtual BOOL ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID);
public:
	virtual ULONG AddDoc(CPDocFactory& pDocFactory,
		CPViewFactroy& pViewFactory);
	virtual Document* Next();
	virtual Document* Pre();
	virtual Document* GetAt(int i);
	virtual BOOL Open(int i);
	virtual BOOL Close(int i);
protected:
	Doc2ViewFactoryList m_rgpDocToView;
	PDoc2View			m_pCurDoc2View;
	LONG m_nCurrent;
};
typedef SharedPtr<DocManager> PDocManager;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Frame : public Widget
{
public:
	Frame();
public:
	virtual Document* GetActiveDoc();
public:
	View* GetActiveView();
protected:
	PWidget m_pMenuBar;
	PWidget m_pStatusBar;
};
typedef SharedPtr<Frame> PFrame;
///////////////////////////*** a gorgeous partition line ***/////////////////////////////
class WFX_API Application : public MsgMap
{
public:
	Application(HINSTANCE hInstance, PFrame pFrame = PFrame());
protected:
	void AddDocument(CPDocFactory& pDocFactory,
		CPViewFactroy& pViewFactory);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
protected:
	HINSTANCE m_hInstance;
	PDocManager m_pDocMgr;
	PFrame m_pFrame;
};
typedef SharedPtr<Application> PApplication;

END_NAMESPACE_WFX
