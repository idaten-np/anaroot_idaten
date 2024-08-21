/*
 * @file TArtDecoderWR.h
 * written by J. Park   for IDATEN 2024
 *--------------------------------------------------------
 */
#ifndef TARTDECODERWR_H
#define TARTDECODERWR_H
#include "TArtDecoder.hh"
class TArtDecoderWR  : public TArtDecoder {
private:
   TArtDecoderWR();

public:
   static const int kID = 62;
   static TArtDecoder* Instance();
   ~TArtDecoderWR();
   int Decode(unsigned char* &buf, const unsigned int& size,
                      TArtRawSegmentObject *rawseg) ;

};
#endif // end of #ifdef TARTDECODERWR_H
