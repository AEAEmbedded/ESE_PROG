/******************************************************************************
 * Project        : Embedded Systems Project en DSB Practicum
 * Copyright : 2010-2020 Original Authors
 ******************************************************************************

 Template voor het bundelen van data.

******************************************************************************/
/* $Id: dataPakket.h 3672 2019-10-29 12:15:09Z dev $                       */

#ifndef ESE_DataPakket_H_
#define ESE_DataPakket_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <algdef.h>

/******************************************************************************
  Defines
******************************************************************************/
const char CR='\r';
const char LF='\n';
const char BACKSPACE='\b';
const char SPACE=' ';
const char ESC=0x1B;

template<typename ttype>
class DataPakket
{
public:

	DataPakket(ttype * const dataPtr,
			   const UInt32 gr) : data(dataPtr),grootte(gr)
	{
		assert(nullptr != dataPtr);
		assert(0 != grootte);
	};

	UInt32 geefGrootte() const
	{
		return(grootte);
	};

	DataPakket<ttype> & operator =  (ttype const * const dataPtr)
	{
		const auto hoeveelheidBytes = sizeof(ttype)*grootte;
		assert(0!= hoeveelheidBytes);
		auto bestemming = reinterpret_cast<UInt8 *>(data);
		const auto bron = reinterpret_cast<UInt8 *>(dataPtr);
		memcpy(bestemming,bron,hoeveelheidBytes);
		return(*this);
	}

	DataPakket<ttype> & operator = (const DataPakket<ttype> &rhs)
	{
		assert(grootte == rhs.grootte);
		const auto kopieGrootte = sizeof(ttype)*grootte;

		auto bestemming = reinterpret_cast<UInt8 *>(data);
		const auto bron = reinterpret_cast<UInt8 *>(rhs.data);
		memcpy(bestemming,bron,kopieGrootte);
		return(*this);
	};

	ttype & operator [] (const UInt32 index) const
	{
		assert(index < grootte);
		return(data[index]);
	};

	/*! @brief geef een pointer voor versturen/ontvangen */
	ttype * geefPtr() const
	{
		return(data);

	};

protected:
	ttype * data;
	const UInt32 grootte;
};


/*! @class Deze klasse definieert een datatransfer tussen microcontroller en pc */
template<typename ttype, const UInt32 Grootte>
class FixedDataPakket : public DataPakket<ttype>
{
public:

	/*! @brief constructor voor static allocatie */
	FixedDataPakket() : DataPakket<ttype>(dataRuimte,Grootte)
	{
		memset(DataPakket<ttype>::data,0,Grootte*sizeof(ttype));
	};

	explicit FixedDataPakket(ttype const * const dataPtr) : DataPakket<ttype>(dataRuimte,Grootte)
	{
		assert(nullptr != dataPtr);
		memcpy(DataPakket<ttype>::data,dataPtr,Grootte*sizeof(ttype));
	};

	using DataPakket<ttype>::operator=;

protected:
	ttype dataRuimte[Grootte];
};


template<typename ttype, const UInt32 Grootte>
class VerzendOntvangBuffer : public FixedDataPakket<ttype,Grootte>
{
public:
	VerzendOntvangBuffer() : FixedDataPakket<ttype,Grootte>()
	{

	};

	explicit VerzendOntvangBuffer(ttype * const dataPtr) : FixedDataPakket<ttype,Grootte>(dataPtr)
	{

	};

	VerzendOntvangBuffer(const VerzendOntvangBuffer<ttype,Grootte> &rhs)
	{
		DataPakket<ttype>::operator = (rhs);
		count = rhs.count;
	}

	using DataPakket<ttype>::operator=;

	void reset()
	{
		count=0;
	};

	UInt32 geefIndex() const
	{
		return(count);
	};

	bool klaar() const
	{
		return(count==Grootte);
	};

	ttype zend(const bool verhoog=true)
	{
		const ttype veld = DataPakket<ttype>::data[count];
		if (true == verhoog)
			count++;
		return(veld);
	};

	bool ontvang(const ttype val,const bool verhoog=true)
	{
		DataPakket<ttype>::data[count] = val;

		if (true == verhoog)
			count++;

		return(klaar());
	};

	/*! @brief forceer dat de buffer klaar is met zenden/ontvangen. */
	void zetKlaar()
	{
		count=Grootte;
	};

private:

	UInt32 count=0;

};



#endif /* DataPakket */
