/*
 * @file TArtDecoderLUPO.h
 * written by T. Isobe   
 *--------------------------------------------------------
 */
#ifndef TARTDECODERLUPO_H
#define TARTDECODERLUPO_H
#include "TArtDecoder.hh"
class TArtDecoderLUPO  : public TArtDecoder {
private:
   TArtDecoderLUPO();

public:
  static const int kID = 8; // JP: temporary switch from 42
   static TArtDecoder* Instance();
   ~TArtDecoderLUPO();
   int Decode(unsigned char* &buf, const unsigned int& size,
                      TArtRawSegmentObject *rawseg) ;
};
#endif // end of #ifdef TARTDECODERLUPO_H
