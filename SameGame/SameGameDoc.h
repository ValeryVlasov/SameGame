
// SameGameDoc.h: интерфейс класса CSameGameDoc 
//


#pragma once
#include "CSameGameBoard.h"
#include <stack>

class CSameGameDoc : public CDocument
{
protected: // создать только из сериализации
	CSameGameDoc() noexcept;
	DECLARE_DYNCREATE(CSameGameDoc)

// Атрибуты
public:

// Операции
public:
	COLORREF GetBoardSpace(int row, int col) { return m_board->GetBoardSpace(row, col); }
	void SetupBoard(void) { return m_board->SetupBoard();}
	void DeleteBoard(void) { return m_board->DeleteBoard(); }


	int GetWidth(void) { return m_board->GetWidth(); }
	void SetWidth(int nWidth) { m_board->SetWidth(nWidth); }

	int GetHeight(void) { return m_board->GetHeight(); }
	void SetHeight(int nHeight) { m_board->SetHeight(nHeight); }

	int GetRows(void) { return m_board->GetRows(); }
	void SetRows(int nRows) { m_board->SetRows(nRows); }

	int GetColumns(void) { return m_board->GetColumns(); }
	void SetColumns(int nColumns) { m_board->SetColumns(nColumns); }

	bool IsGameOver() { return m_board->IsGameOver(); }
	int DeleteBlocks(int row, int col);
	int GetRemainingCount() { return m_board->GetRemainingCount(); }

	void SetNumColors(int nColors);
	int GetNumColors(void) { return m_board->GetNumColors(); }

// Реализация
public:
	virtual ~CSameGameDoc();

protected:
	CSameGameBoard* m_board;
	void ClearUndo();
	void ClearRedo();
	std::stack<CSameGameBoard*> m_undo;// Стек "Отмена"
	std::stack<CSameGameBoard*> m_redo;// Стек "Повтор"
public:
	bool CanRedo();
	bool CanUndo();
	void RedoLast();
	void UndoLast();


	// Переопределение
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS



};
