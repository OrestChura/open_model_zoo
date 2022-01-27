// Copyright (C) 2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include <utils_gapi/stream_source.hpp>

namespace custom {
CommonCapSrc::CommonCapSrc(std::shared_ptr<ImagesCapture>& imagesCapture)
    : cap(imagesCapture) {
    preparation();
}

void CommonCapSrc::preparation() {
    GAPI_Assert(first.empty());
    cv::Mat tmp = cap->read();
    if (!tmp.data) {
        GAPI_Assert(false && "Couldn't grab the first frame");
    }
    first = tmp;
}

bool CommonCapSrc::pull(cv::gapi::wip::Data &data) {
    if (!first_pulled) {
        GAPI_Assert(!first.empty());
        first_pulled = true;
        data = first.clone();
        return true;
    }
    cv::Mat current_frame = cap->read();
    if (!current_frame.data) {
        return false;
    }
    GAPI_Assert(cv::descr_of(first) == cv::descr_of(current_frame));
    data = current_frame.clone();
    return true;
}

cv::GMetaArg CommonCapSrc::descr_of() const {
    GAPI_Assert(!first.empty());
    return cv::GMetaArg{ cv::descr_of(first) };
}
} // namespace custom
