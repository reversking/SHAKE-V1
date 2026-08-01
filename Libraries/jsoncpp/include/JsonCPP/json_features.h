




#ifndef JSON_FEATURES_H_INCLUDED
#define JSON_FEATURES_H_INCLUDED

#if !defined(JSON_IS_AMALGAMATION)
#include "forwards.h"
#endif

#pragma pack(push)
#pragma pack()

namespace Json {





class JSON_API Features {
public:






  static Features all();







  static Features strictMode();



  Features();


  bool allowComments_{true};



  bool strictRoot_{false};


  bool allowDroppedNullPlaceholders_{false};


  bool allowNumericKeys_{false};
};

}

#pragma pack(pop)

#endif
