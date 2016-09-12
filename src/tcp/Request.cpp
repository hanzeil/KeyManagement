//
// Created by root on 16-9-12.
//
// Author root.
//
#include "Request.h"

namespace http {
    namespace server {

        void Request::Reset() {
            data.clear();
        }
    }
}
