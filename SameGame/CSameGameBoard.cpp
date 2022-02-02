#include "pch.h"
#include "CSameGameBoard.h"


CSameGameBoard::CSameGameBoard(void)
	: m_arrBoard(NULL), m_nColumns(15),
	m_nRows(15), m_nHeight(35), m_nWidth(35), m_nRemaining(0)
{
	m_arrColors[0] = RGB(0, 0, 0);
	m_arrColors[1] = RGB(255, 0, 0);
	m_arrColors[2] = RGB(0, 255, 0);
	m_arrColors[3] = RGB(0, 0, 255);

	m_arrColors[4] = RGB(255, 255, 64);
	m_arrColors[5] = RGB(0, 255, 255);
	m_arrColors[6] = RGB(255, 0, 128);
	m_arrColors[7] = RGB(0, 64, 0);

	SetupBoard();// Создание и настройка параметров игровой доски
}

CSameGameBoard::CSameGameBoard(const CSameGameBoard& board)
{
	// Копирование всех элементов класса
	m_nColumns = board.m_nColumns;
	m_nRows = board.m_nRows;
	m_nHeight = board.m_nHeight;
	m_nWidth = board.m_nWidth;
	m_nRemaining = board.m_nRemaining;
	m_nColors = board.m_nColors;

	// Копирование цветовых элементов
	for (int i = 0; i < 8; i++)
		m_arrColors[i] = board.m_arrColors[i];
	m_arrBoard = NULL;

	// Создание новой игровой доски
	CreateBoard();

	// Копирование содержимого игрового поля
	for (int row = 0; row < m_nRows; row++)
		for (int col = 0; col < m_nColumns; col++)
			m_arrBoard[row][col] = board.m_arrBoard[row][col];
}

CSameGameBoard::~CSameGameBoard(void)
{
	DeleteBoard();
}

void CSameGameBoard::SetupBoard(void)
{
	if (m_arrBoard == NULL)// При необходимости создаем доску
		CreateBoard();

	for (int row = 0; row < m_nRows; row++)// Устанавливаем каждому блоку случайный цвет
		for (int col = 0; col < m_nColumns; col++)
			m_arrBoard[row][col] = (rand() % m_nColors) + 1;

	m_nRemaining = m_nRows * m_nColumns;// Устанавливаем количество оставшихся блоков
}

void CSameGameBoard::DeleteBoard(void)
{
	if (m_arrBoard != NULL)
	{
		for (int row = 0; row < m_nRows; row++)
			if (m_arrBoard[row] != NULL)
			{
				delete[] m_arrBoard[row];
				m_arrBoard[row] = NULL;
			}
		delete[] m_arrBoard;
		m_arrBoard = NULL;
	}
}

COLORREF CSameGameBoard::GetBoardSpace(int row, int col)
{
	if (row < 0 || row >= m_nRows || col < 0 || col >= m_nColumns)
		return m_arrColors[0];
	return m_arrColors[m_arrBoard[row][col]];
}

void CSameGameBoard::CreateBoard(void)
{
	if (m_arrBoard != NULL)//Если осталось поле с предыдущего раза, то удаляем его
		DeleteBoard();

	m_arrBoard = new int* [m_nRows];//Т.к. память поля очищена, то выделяем её снова

	for (int row = 0; row < m_nRows; row++)
	{
		m_arrBoard[row] = new int[m_nColumns];
		for (int col = 0; col < m_nColumns; col++)
			m_arrBoard[row][col] = 0;//Заполняем все ячейки цветом фона
	}
}

int CSameGameBoard::DeleteBlocks(int row, int col)
{
	if (row < 0 || row >= m_nRows || col < 0 || col >= m_nColumns)
		return -1;

	int nColor = m_arrBoard[row][col];// Если блок уже имеет цвет фона, то удалить его не получится
	if (nColor == 0)
		return -1;

	int nCount = -1;// Сначала проверяем, если ли примыкающие блоки с тем же цветом
	if ((row - 1 >= 0 && m_arrBoard[row - 1][col] == nColor) ||
		(row + 1 < m_nRows && m_arrBoard[row + 1][col] == nColor) ||
		(col - 1 >= 0 && m_arrBoard[row][col - 1] == nColor) ||
		(col + 1 < m_nColumns && m_arrBoard[row][col + 1] == nColor))
	{
		// Затем рекурсивно вызываем функцию для удаления примыкающих блоков одного цвета
		m_arrBoard[row][col] = 0;
		nCount = 1;

		//...сверху...
		nCount += DeleteNeighborBlocks(row - 1, col, nColor, DIRECTION_DOWN);

		//...снизу...
		nCount += DeleteNeighborBlocks(row + 1, col, nColor, DIRECTION_UP);

		//...слева...
		nCount += DeleteNeighborBlocks(row, col - 1, nColor, DIRECTION_RIGHT);

		//...справа...
		nCount += DeleteNeighborBlocks(row, col + 1, nColor, DIRECTION_LEFT);

		CompactBoard();// В конце выполняем «сжатие» нашей игровой доски

		m_nRemaining -= nCount;// Вычитаем число удаленных блоков из общего количества блоков
	}
	return nCount;// Возвращаем количество удаленных блоков
}

int CSameGameBoard::DeleteNeighborBlocks(int row, int col, int color, Direction direction)
{
	// Проверяем на корректность индексы ячейки и столбца
	if (row < 0 || row >= m_nRows || col < 0 || col >= m_nColumns)
		return 0;

	// Проверка на то, что блок имеет тот же цвет
	if (m_arrBoard[row][col] != color)
		return 0;

	int nCount = 1;
	m_arrBoard[row][col] = 0;

	// Выполняем проверку направлений
	if (direction != DIRECTION_UP)//  Если мы пришли НЕ СВЕРХУ, то идем НАВЕРХ
		nCount += DeleteNeighborBlocks(row - 1, col, color, DIRECTION_DOWN);

	if (direction != DIRECTION_DOWN)//  Если мы пришли НЕ СНИЗУ, то спускаемся ВНИЗ
		nCount += DeleteNeighborBlocks(row + 1, col, color, DIRECTION_UP);

	if (direction != DIRECTION_LEFT)//  Если мы пришли НЕ СЛЕВА, то идем ВЛЕВО
		nCount += DeleteNeighborBlocks(row, col - 1, color, DIRECTION_RIGHT);

	if (direction != DIRECTION_RIGHT)//  Если мы пришли НЕ СПРАВА, то идем ВПРАВО
		nCount += DeleteNeighborBlocks(row, col + 1, color, DIRECTION_LEFT);

	return nCount;// Возвращаем общее количество удаленных блоков
}

void CSameGameBoard::CompactBoard(void)
{
	for (int col = 0; col < m_nColumns; col++)
	{
		int nNextEmptyRow = m_nRows - 1;
		int nNextOccupiedRow = nNextEmptyRow;
		while (nNextOccupiedRow >= 0 && nNextEmptyRow >= 0)//nNextOccupiedRow & nNextEmptyRow >= 0
		{
			while (nNextEmptyRow >= 0 && m_arrBoard[nNextEmptyRow][col] != 0)
				nNextEmptyRow--;
			if (nNextEmptyRow >= 0)
			{
				nNextOccupiedRow = nNextEmptyRow - 1;
				while (nNextOccupiedRow >= 0 && m_arrBoard[nNextOccupiedRow][col] == 0)
					nNextOccupiedRow--;
				if (nNextOccupiedRow >= 0)
				{
					m_arrBoard[nNextEmptyRow][col] = m_arrBoard[nNextOccupiedRow][col];
					m_arrBoard[nNextOccupiedRow][col] = 0;
				}
			}
		}
	}

	int nNextEmptyCol = 0;
	int nNextOccupiedCol = nNextEmptyCol;
	while (nNextEmptyCol < m_nColumns && nNextOccupiedCol < m_nColumns)
	{
		while (nNextEmptyCol < m_nColumns && m_arrBoard[m_nRows - 1][nNextEmptyCol] != 0)
			nNextEmptyCol++;
		if (nNextEmptyCol < m_nColumns)
		{
			nNextOccupiedCol = nNextEmptyCol + 1;
			while (nNextOccupiedCol < m_nColumns && m_arrBoard[m_nRows - 1][nNextOccupiedCol] == 0)
				nNextOccupiedCol++;
			if (nNextOccupiedCol < m_nColumns)
			{
				for (int row = 0; row < m_nRows; row++)
				{
					m_arrBoard[row][nNextEmptyCol] = m_arrBoard[row][nNextOccupiedCol];
					m_arrBoard[row][nNextOccupiedCol] = 0;
				}
			}
		}
	}
}

bool CSameGameBoard::IsGameOver(void) const
{
	for (int col = 0; col < m_nColumns; col++)
		for (int row = m_nRows - 1; row >= 0; row--)
		{
			int nColor = m_arrBoard[row][col];

			if (nColor == 0)
				break;
			else
			{
				if (row - 1 >= 0 && m_arrBoard[row - 1][col] == nColor)
					return false;
				else if (col + 1 < m_nColumns && m_arrBoard[row][col + 1] == nColor)
					return false;
			}
		}
	return true;
}

