// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < -1) {
        throw "Negative length";
    }
    if (len == -1) {
        len = 0;
    }
    BitLen = len;
    if (len == 0) {
        MemLen = 0;
        pMem = nullptr;
    }
    else {
        int bitsInTask = sizeof(TELEM) * 8;
        MemLen = (len + bitsInTask - 1) / bitsInTask;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = 0;
        }
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    if (MemLen > 0) {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    else {
        pMem = nullptr;
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen) {
        throw "Index out of bounds"; 
    }
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    int position = n % (sizeof(TELEM) * 8);
    return (TELEM)1u << position;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) {
        throw "Index out of bounds";
    }
    int idx = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[idx] = pMem[idx] | mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) {
        throw "Index out of bounds";
    }
    int idx = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[idx] = pMem[idx] & (~mask);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) {
        throw "Index out of bounds";
    }
    int idx = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return (pMem[idx] & mask) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf) {
        if (MemLen != bf.MemLen) {
            delete[] pMem;
            MemLen = bf.MemLen;
            if (MemLen > 0) {
                pMem = new TELEM[MemLen];
            }
            else {
                pMem = nullptr;
            }
        }
        BitLen = bf.BitLen;
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 0;
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 1;

    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) return 1;
    }
    return 0;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    TBitField temp(maxLen);
    for (int i = 0; i < minMemLen; i++) {
        temp.pMem[i] = pMem[i] | bf.pMem[i];
    }
    if (MemLen > bf.MemLen) {
        for (int i = minMemLen; i < MemLen; i++) {
            temp.pMem[i] = pMem[i];
        }
    }
    else {
        for (int i = minMemLen; i < bf.MemLen; i++) {
            temp.pMem[i] = bf.pMem[i];
        }
    }
    return temp;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    TBitField temp(maxLen);
    for (int i = 0; i < minMemLen; i++) {
        temp.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField temp(BitLen);
    for (int i = 0; i < BitLen; i++) {
        if (!GetBit(i)) {
            temp.SetBit(i);
        }
    }
    return temp;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    char ch;
    for (int i = 0; i < bf.GetLength(); i++) {
        istr >> ch;
        if (ch == '1') {
            bf.SetBit(i);
        }
        else if (ch == '0') {
            bf.ClrBit(i);
        }
        else {
            break;
        }
    }
    return istr;
}
ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++) {
        if (bf.GetBit(i)) {
            ostr << '1';
        }
        else {
            ostr << '0';
        }
    }
    return ostr;
}