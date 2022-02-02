#pragma once
class CSameGameBoard
{
private:
	int m_nWidth;
	int m_nHeight;
	int m_nColumns;
	int m_nRows;

	int m_nRemaining;

	int m_nColors;
	
	COLORREF m_arrColors[8];// Список цветов: 0 – это цвет фона, 1-7 – это цвета блоков

	int** m_arrBoard;

	void CreateBoard(void);

	enum Direction// Перечисление с вариантами направления (откуда мы пришли) потребуется для удаления блоков
	{
		DIRECTION_UP,
		DIRECTION_DOWN,
		DIRECTION_LEFT,
		DIRECTION_RIGHT,
	};

	// Вспомогательная рекурсивная функция для удаления примыкающих блоков
	int DeleteNeighborBlocks(int row, int col, int color, Direction direction);

	void CompactBoard(void);// Функция для сжатия доски после того, как были удалены блоки
public:
	CSameGameBoard(void);

	CSameGameBoard(const CSameGameBoard& board);
	
	~CSameGameBoard(void);

	void SetupBoard(void);// Функция для рандомной расстановки блоков в начале игры

	void DeleteBoard(void);

	bool IsGameOver(void) const;

	int GetRemainingCount(void) const { return m_nRemaining; }

	int DeleteBlocks(int row, int col);

	COLORREF GetBoardSpace(int row, int col);// Получаем цвет в определенном участке игрового поля

	int GetWidth(void) const { return m_nWidth; }
	void SetWidth(int nWidth) { m_nWidth = (nWidth >= 3) ? nWidth : 3; }
	
	int GetHeight(void) const { return m_nHeight; }
	void SetHeight(int nHeight) { m_nHeight = (nHeight >= 3) ? nHeight : 3; }
	
	int GetColumns(void) const { return m_nColumns; }
	void SetColumns(int nColumns) { m_nColumns = (nColumns >= 5) ? nColumns : 5; }
	
	int GetRows(void) const { return m_nRows; }
	void SetRows(int nRows) { m_nRows = (nRows >= 5) ? nRows : 5; }

	int GetNumColors(void) { return m_nColors; }
	void SetNumColors(int nColors) { m_nColors = (nColors >= 3 && nColors <= 7) ? nColors : m_nColors; }
};

