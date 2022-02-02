
// SameGameView.cpp: реализация класса CSameGameView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "SameGame.h"
#endif

#include "SameGameDoc.h"
#include "SameGameView.h"
#include "COptionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSameGameView

IMPLEMENT_DYNCREATE(CSameGameView, CView)

BEGIN_MESSAGE_MAP(CSameGameView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()

	ON_COMMAND(ID_LEVEL_3COLORS, &CSameGameView::OnLevel3colors)
	ON_COMMAND(ID_LEVEL_4COLORS, &CSameGameView::OnLevel4colors)
	ON_COMMAND(ID_LEVEL_5COLORS, &CSameGameView::OnLevel5colors)
	ON_COMMAND(ID_LEVEL_6COLORS, &CSameGameView::OnLevel6colors)
	ON_COMMAND(ID_LEVEL_7COLORS, &CSameGameView::OnLevel7colors)

	ON_UPDATE_COMMAND_UI(ID_LEVEL_3COLORS, &CSameGameView::OnUpdateLevel3colors)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_4COLORS, &CSameGameView::OnUpdateLevel4colors)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_5COLORS, &CSameGameView::OnUpdateLevel5colors)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_6COLORS, &CSameGameView::OnUpdateLevel6colors)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_7COLORS, &CSameGameView::OnUpdateLevel7colors)

	ON_COMMAND(ID_SETUP_BLOCKCOUNT, &CSameGameView::OnSetupBlockcount)
	ON_COMMAND(ID_SETUP_BLOCKSIZE, &CSameGameView::OnSetupBlocksize)

	ON_COMMAND(ID_32771, &CSameGameView::On32771)
	ON_COMMAND(ID_EDIT_UNDO, &CSameGameView::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CSameGameView::OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_32771, &CSameGameView::OnUpdate32771)
END_MESSAGE_MAP()

// Создание или уничтожение CSameGameView

CSameGameView::CSameGameView() noexcept
{
	// TODO: добавьте код создания

}

CSameGameView::~CSameGameView()
{
}

BOOL CSameGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CSameGameView

void CSameGameView::OnDraw(CDC* pDC)
{
	CSameGameDoc* pDoc = GetDocument();// В начале создаем указатель на Document
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int nDCSave = pDC->SaveDC();// Сохраняем текущее состояние контекста устройства

	CRect rcClient;
	GetClientRect(&rcClient);// Получаем размеры клиентской области
	COLORREF clr = pDoc->GetBoardSpace(-1, -1);

	pDC->FillSolidRect(&rcClient, clr);// Сначала отрисовываем фон

	CBrush br;// Создаем кисть для рисования
	br.CreateStockObject(HOLLOW_BRUSH);
	CBrush* pbrOld = pDC->SelectObject(&br);

	for (int row = 0; row < pDoc->GetRows(); row++)//Рисуем блоки игрового пространства
	{
		for (int col = 0; col < pDoc->GetColumns(); col++)
		{
			clr = pDoc->GetBoardSpace(row, col);

			CRect rcBlock;
			rcBlock.top = row * pDoc->GetHeight();
			rcBlock.bottom = rcBlock.top + pDoc->GetHeight();
			rcBlock.left = col * pDoc->GetWidth();
			rcBlock.right = rcBlock.left + pDoc->GetWidth();

			pDC->FillSolidRect(&rcBlock, clr);// Заполняем блок соответствующим цветом

			pDC->Rectangle(&rcBlock);// Рисуем контур
		}
	}
	pDC->RestoreDC(nDCSave);// Восстанавливаем контекст устройства
	br.DeleteObject();
}


// Диагностика CSameGameView

#ifdef _DEBUG
void CSameGameView::AssertValid() const
{
	CView::AssertValid();
}

void CSameGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSameGameDoc* CSameGameView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSameGameDoc)));
	return (CSameGameDoc*)m_pDocument;
}
#endif //_DEBUG

void CSameGameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	ResizeWindow();
}

void CSameGameView::ResizeWindow()
{
	CSameGameDoc* pDoc = GetDocument();// Создаем указатель на Document
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Получаем размеры клиентской области
	CRect rcClient, rcWindow;
	GetClientRect(&rcClient);
	GetParentFrame()->GetWindowRect(&rcWindow);
	int nWidthDiff = rcWindow.Width() - rcClient.Width();
	int nHeightDiff = rcWindow.Height() - rcClient.Height();

	// Изменяем размеры окна, исходя из размеров нашей доски
	rcWindow.right = rcWindow.left + pDoc->GetWidth() * pDoc->GetColumns() + nWidthDiff;
	rcWindow.bottom = rcWindow.top + pDoc->GetHeight() * pDoc->GetRows() + nHeightDiff;

	GetParentFrame()->MoveWindow(&rcWindow);// Функция MoveWindow() изменяет размер окна фрейма
}

void CSameGameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Вначале создаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Получаем индекс строки и столбца элемента, по которому был осуществлен клик мышкой
	int row = point.y / pDoc->GetHeight();
	int col = point.x / pDoc->GetWidth();

	int count = pDoc->DeleteBlocks(row, col);// Удаляем блоки из Document

	if (count > 0)// Проверяем, было ли удаление блоков
	{
		Invalidate();// Перерисовываем View
		UpdateWindow();

		if (pDoc->IsGameOver())
		{
			int remaining = pDoc->GetRemainingCount();// Получаем количество оставшихся блоков
			CString message;
			message.Format(_T("No more moves left\nBlocks remaining: %d"), remaining);

			MessageBox(message, _T("Game Over"), MB_OK | MB_ICONINFORMATION);// Отображаем пользователю результат игры
		}
	}

	CView::OnLButtonDown(nFlags, point);// OnLButtonDown по умолчанию
}


// Обработчики сообщений CSameGameView
void CSameGameView::setColorCount(int numColors)
{
	// Сначала получаем указатель на документ
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// Устанавливаем количество цветов 
	pDoc->SetNumColors(numColors);
	// Перерисовываем View
	Invalidate();
	UpdateWindow();
}

void CSameGameView::OnLevel3colors()
{
	setColorCount(3);
}


void CSameGameView::OnLevel4colors()
{
	setColorCount(4);
}


void CSameGameView::OnLevel5colors()
{
	setColorCount(5);
}


void CSameGameView::OnLevel6colors()
{
	setColorCount(6);
}


void CSameGameView::OnLevel7colors()
{
	setColorCount(7);
}

void CSameGameView::UpdateColorCount(int numColors, CCmdUI* pCmdUI)
{
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pCmdUI->SetCheck(pDoc->GetNumColors() == numColors);
}


void CSameGameView::OnUpdateLevel3colors(CCmdUI* pCmdUI)
{
	UpdateColorCount(3, pCmdUI);
}


void CSameGameView::OnUpdateLevel4colors(CCmdUI* pCmdUI)
{
	UpdateColorCount(4, pCmdUI);
}


void CSameGameView::OnUpdateLevel5colors(CCmdUI* pCmdUI)
{
	UpdateColorCount(5, pCmdUI);
}


void CSameGameView::OnUpdateLevel6colors(CCmdUI* pCmdUI)
{
	UpdateColorCount(6, pCmdUI);
}


void CSameGameView::OnUpdateLevel7colors(CCmdUI* pCmdUI)
{
	UpdateColorCount(7, pCmdUI);
}


void CSameGameView::OnSetupBlockcount()
{
	// Получаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Создаём диалоговое окно
	COptionDialog dlg(true, this);

	// Устанавливаем параметры строк и столбцов
	dlg.m_nValue1 = pDoc->GetRows();
	dlg.m_nValue2 = pDoc->GetColumns();

	if (dlg.DoModal() == IDOK)// Отображаем полученное окно
	{
		// Удаляем игровое поле
		pDoc->DeleteBoard();

		// Устанавливаем введенные пользователем параметры
		pDoc->SetRows(dlg.m_nValue1);
		pDoc->SetColumns(dlg.m_nValue2);

		pDoc->SetupBoard();// Обновляем игровую доску

		ResizeWindow();// Изменяем размеры View
	}
}


void CSameGameView::OnSetupBlocksize()
{
	// Получаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Создаём диалоговое окно
	COptionDialog dlg(false, this);

	// Устанавливаем параметры строк и столбцов
	dlg.m_nValue1 = pDoc->GetWidth();
	dlg.m_nValue2 = pDoc->GetHeight();

	if (dlg.DoModal() == IDOK)// Отображаем полученное окно
	{
		// Удаляем игровое поле
		pDoc->DeleteBoard();

		// Устанавливаем введенные пользователем параметры
		pDoc->SetWidth(dlg.m_nValue1);
		pDoc->SetHeight(dlg.m_nValue2);

		pDoc->SetupBoard();// Обновляем игровую доску

		ResizeWindow();// Изменяем размеры View
	}
}


void CSameGameView::OnEditUndo()
{
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->UndoLast();

	Invalidate();
	UpdateWindow();
}

void CSameGameView::On32771()
{
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->RedoLast();

	Invalidate();
	UpdateWindow();
}

void CSameGameView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pCmdUI->Enable(pDoc->CanUndo());
}

void CSameGameView::OnUpdate32771(CCmdUI* pCmdUI)
{
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pCmdUI->Enable(pDoc->CanRedo());
}
