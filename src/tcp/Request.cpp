//
// Created by root on 16-9-12.
//
// Author root.
//
#include "Request.h"

namespace tcp {

    void Request::Reset() {
        method.clear();
        length = 0;
        data.clear();
    }
}
