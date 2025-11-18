
/************************************************************************/
/* Company                                                            */
/* Algemene definities voor operatie                                    */
/* Alle software in dit bestand is                                      */
/* (c) 2010-2020 Original Authors
/* $Id: algdef.h 3676 2019-10-30 16:30:33Z dev $                                                                 */
/************************************************************************/

#ifndef ESE_ALGDEF_H
#define ESE_ALGDEF_H


#include <stdio.h>
#include <stdint.h>
#include <math.h>

/* standaard definities */

#ifndef PI
static constexpr float  PI = 3.141592654f;
#endif

#ifdef  FALSE
#define False		FALSE
#else
#define False		false
#endif

#ifdef  TRUE
#define True		TRUE
#else
#define True		true
#endif

#ifndef NULL
#define NULL            nullptr
#endif

#define ALTIJD   while(1==1)
#define FOREVER  ALTIJD

#ifndef _WIN32
/*! @brief om aan te geven data deze funktie verouderd is en vervangen moet worden. */
#define VEROUDERD   __attribute__((deprecated))
/*! @brief voor het geforceerd linken van een ongebruikte funktie. */
#define GEBRUIKT   __attribute__((used))
/*! @brief voor test, dan geen waarschuwing (fout) bij het compileren. */
#define ONGEBRUIKT   __attribute__((unused))
#define ZWAKGEPLAATST __attribute__((weak))
#define GEENTERUGKEER __attribute__((__noreturn__))
#else
#define VEROUDERD
#define GEBRUIKT
#define ONGEBRUIKT
#define GEENTERUGKEER 
#endif

/* Basic Types
 */
typedef char	 		Char;
typedef unsigned char		UChar;
typedef unsigned int		UInt;
typedef int			Int;
typedef float			Float;
typedef double			Double;

/* Sized Types
 */
typedef signed char	  Int8;
typedef uint8_t	  UInt8;
typedef int16_t	  Int16;
typedef uint16_t  UInt16;
typedef int32_t   Int32;
typedef uint32_t  UInt32;
typedef int64_t		Int64;
typedef uint64_t	UInt64;

/* algemene funktie foutmelding */

enum class FoutCode
{
    Ok=0,
    Waarschuwing,  /* Warning */
    Fout,          /* Error */
    Ernstig,       /* Error (serious) */
    Rampzalig,      /* Error (desastrous) */
    ReserveFoutCode,  /* Reserve error code */
    Onbekend      /* unknown */
};

using FC = FoutCode;

/*!@ @brief English variant of the error code */
using ErrorCode = FoutCode;

enum class Schakelaar
{
    Uit=0,  /* Off */
    Aan,    /* On */
	Om     /* Toggle */
   // Einde   /* Last one */
} ;

/*!@ @brief English variant of the switch */
using Switch = Schakelaar;


typedef UInt32 Teller;             /* standaard breedte van de ARM7 */ 
typedef Int32 STeller;             /* standaard breedte van de ARM7 */ 
typedef float WiskundeFloatType;   /* schakel tussen enkel en dubbel precisie */
typedef WiskundeFloatType WFT;


/* funkties en inline funkties */

/* bitsgewijze operatie macros . Geef bits aan als nummer en niet als 2 tot de macht (nummer) */
template <typename ttype>
void SetBit(ttype &x,const Teller y)
{
	static const ttype schuiver = 1;
	const ttype verschuiving = (schuiver << y);
	x |= verschuiving;       /* Set bit y in byte x*/
}

//template <typename ttype1,typename ttype2>
//void SetBit(ttype1 &x,const ttype2 y)
//{
//    x |= (1<<y);       /* Set bit y in byte x*/
//}
//

template <typename ttype>
void SetBit(ttype * const x,const Teller y)
{
	static const ttype schuiver = 1;
	const ttype verschuiving = (schuiver << y);
	ttype inhoud = *x;
	inhoud |= verschuiving;       /* Set bit y in byte x*/
	*x = inhoud;
}

template <typename ttype>
void SetBitM(ttype &x,const ttype y)
{
	x |= y;       /* Set bit y in byte x*/
}

template <typename ttype>
void SetBitM(ttype * const x,const ttype y)
{
	ttype inhoud = *x;
	inhoud |= y;       /* Set bit y in byte x*/
	*x = inhoud;
}

template <typename ttype>
void ClearBit(ttype &x,const Teller y)
{
	static const ttype schuiver = 1;
	const ttype verschuiving = ~(schuiver << y);
	x &= verschuiving;       /* Clear bit y in byte x*/
}

template <typename ttype>
void ClearBit(ttype * const x,const Teller y)
{
	static const ttype schuiver = 1;
	const ttype verschuiving = ~(schuiver << y);
	ttype inhoud = *x;
	inhoud &= verschuiving;       /* Clear bit y in byte x*/
	*x = inhoud;
}

template <typename ttype>
void ClearBitM(ttype &x,const ttype y)
{
	x &= (~y);       /* Clear bit y in byte x*/
}

template <typename ttype>
bool CheckBit(const ttype x,const Teller y)
{
	static const ttype schuiver=1;
	const ttype verschuiving = static_cast<ttype>(schuiver << y);
	return(((0==(x & verschuiving)) ? false : true));
}

template <typename ttype>
bool InvCheckBit(const ttype x,const Teller y)
{
	static const ttype schuiver=1;
	const ttype verschuiving = static_cast<ttype>(schuiver << y);
	return(((0==(x & verschuiving)) ? true : false));
}

template <typename ttype>
bool CheckBit(ttype const * const x,const Teller y)
{
	static const ttype schuiver=1;
	const ttype verschuiving = static_cast<ttype>(schuiver << y);
	return(((0==((*x) & verschuiving)) ? false : true));
}

template <typename ttype>
bool CheckBitM(const ttype x,const ttype y)
{
	return(((0==(x & y)) ? false : true));
}

template <typename ttype>
bool CheckBitM(const ttype *x,const ttype y)
{
	return(((0==((*x) & y)) ? false : true));
}


/*! @brief klap het bit op nummer y om */
template <typename ttype>
void ToggleBit(ttype &x,const Teller y)
{
	static const ttype schuiver=1;
	const ttype verschuiving = (schuiver << y);
	x ^=  verschuiving;  /* verander bit y in byte x */
}

/*! @brief klap het bitpatroon y om */
template <typename ttype>
void ToggleBitM(ttype &x,const Teller y)
{
	x ^= y;  /* verander bit y in byte x */
}

/*! @brief klap het bit op nummer y om */
template <typename ttype>
void ToggleBit(ttype *x,const Teller y)
{
	static const ttype schuiver = 1;
	const ttype verschuiving = (schuiver << y);
	ttype inhoud = *x;
	inhoud ^= verschuiving;       /* toggle bit y in byte x*/
	*x = inhoud;
}

/*! @brief Zet bit y in byte x gelijk aan de boolean b */
template <typename ttype>
void EnterBit(ttype &x,const Teller y,const bool b)
{
	if (true==b)
		SetBit<ttype>(x,y);
	else
		ClearBit<ttype>(x,y);
}

/*! @brief zet bitpatroon y in x gelijk aan de boolean b */
template <typename ttype>
void EnterBitM(ttype &x,const ttype y,const bool b)
{
	if (true==b)
		SetBitM<ttype>(x,y);
	else
		ClearBitM<ttype>(x,y);
}



/*! @brief zet bit y in byte x aan als Schakelaar==aan, anders uit */
template <typename ttype>
void EnterBit(ttype &x,const Teller y,const Schakelaar knop)
{
	if (knop==Schakelaar::Aan)
		SetBit<ttype>(x,y);
	else
		ClearBit<ttype>(x,y);
}

/* tel b bij a op binnen 'bits' aantal bits. Bij een overflow over 'bits' bits wordt slechts het restant doorgegeven */
/* Werkt alleen bij unsigned integers !!! */
#define CompanyTelBinnenBits(a,b, bits)  ( ( (a+b) < (1<<bits) ) ? a+b : (a+b-1-(1<<bits)))  

    
/* Bit definities */
#define KBit0                0x01
#define KBit1                0x02
#define KBit2                0x04
#define KBit3                0x08
#define KBit4                0x10
#define KBit5                0x20
#define KBit6                0x40
#define KBit7                0x80


/* Serieele hulpjes */
#define HoogWoord(woord)    ((UInt16) 0xffff & (woord >> 16))  /* In een 32 bit geheugen, is dit het Most Significant Word (16 bit) */
#define LaagWoord(woord)    ((UInt16) 0xffff & woord)          /* In een 32 bit geheugen, is dit het Least Significant Word (16 bit) */
#define HoogByte(woord)    ((UInt8) 0xff & (woord >> 8))       /* In een 16 bit geheugen, is dit het Most Significant Byte (8 bit) */
#define LaagByte(woord)    ((UInt8) 0xff & woord)              /* In een 16 bit geheugen, is dit het Least Significant Byte (8 bit) */

#define ReturnIsOk(a)  ( ( (a) == Ok ) ? True : False )
#define ReturnIsNietOk(a)  ( ( (a) != Ok ) ? True : False )

typedef enum
{
    BreukRelNummers_0=0,
    BreukRelNummers_1,
    BreukRelNummers_2,
    BreukRelNummers_3,
    BreukRelNummers_4,
    BreukRelNummers_5,
    BreukRelNummers_6,
    BreukRelNummers_7,
    BreukRelNummers_8,
    BreukRelNummers_Einde
} BreukRelAantalRelevanteNummers;

class Breuk
{
public:
    Breuk(const Int16 t,     /* de teller */
          const Int16 n=1);  /* de noemer. standaard = 1 --> geheel getal */

    Breuk(const WiskundeFloatType,          /* input: floating point getal */
	  const BreukRelAantalRelevanteNummers );           /* aantal relevante cijfers in het drijvende komma getal */

    bool operator != (const Breuk &) const;
    bool operator == (const Breuk &) const;

    Breuk operator + (const Breuk &) const;

    Breuk operator + (const Int16 ) const;
  
    Breuk operator - (const Breuk &) const;

    Breuk operator - (const Int16 ) const;

    Breuk & operator += (const Breuk &);
    Breuk & operator -= (const Breuk &);
   
    Breuk & operator *= (const Int16);
    Breuk & operator *= (const Breuk &);

    Breuk  operator * (const Int16) const;
    Breuk  operator * (const Breuk &) const;

    Breuk & operator /= (const Int16);
    Breuk  operator / (const Int16) const;
    Breuk  operator / (const Breuk &) const;


    Int16 teller() const
        {
            return(tel);
        }

    Int16 noemer() const
        {
            return(noem);
        }
    
    Int16 vermenigvuldig(const Int16) const; /* vermenigvuldig breuk met Int16 en rond af naar dichtbijzijnde Int16*/

    float geefFloat() const;  /* geefType breuk als float */
 
    Int16 rondAf() const;   /* rond breuk af naar integer */
    
protected:
    void normaliseer();

private:
    Int16 tel,noem;
    static const Int32 nulWerkers[];
};

/*! @Brefi English typedef of Breuk */
using Fraction = Breuk;

GEENTERUGKEER void StopHier();







#endif  /* ALGDEF_H */





