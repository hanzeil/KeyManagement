//
// Created by Hanzeil on 16-9-4.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#ifndef KEYMANAGEMENT_MIME_TYPES_H
#define KEYMANAGEMENT_MIME_TYPES_H

#include <string>

namespace http {
    namespace server {
        namespace mime_types {

/// Convert a file extension into a MIME type.
            std::string extension_to_type(const std::string &extension);

        } // namespace mime_types
    } // namespace server
} // namespace http


#endif //KEYMANAGEMENT_MIME_TYPES_H
