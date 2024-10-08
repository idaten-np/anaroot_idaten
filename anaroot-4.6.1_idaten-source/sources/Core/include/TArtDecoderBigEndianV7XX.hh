/*
 * @file TArtDecoderBigEndianV7XX.h
 */
#ifndef TARTDECODERBIGENDIANV7XX_H
#define TARTDECODERBIGENDIANV7XX_H
#include "TArtDecoder.hh"
class TArtDecoderBigEndianV7XX  : public TArtDecoder {
private:
   TArtDecoderBigEndianV7XX();

public:
   static const int kID = 27;
   static TArtDecoder* Instance();
   ~TArtDecoderBigEndianV7XX();
   int Decode(unsigned char* &buf, const unsigned int& size,
	      TArtRawSegmentObject *rawseg);

};
#endif // end of #ifdef TARTDECODERBigEndianV7XX_H
