/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * libiio - Library for interfacing industrial I/O (IIO) devices
 *
 * Copyright (C) 2023, DIFITEC GmbH
 * Author: Tilman Blumhagen <tilman.blumhagen@difitec.de>
 */

/** @file iiopp.cpp
 * @brief Public C++ interface
 *
 * @see @ref iiopp
*/

#include "iiopp.h"
namespace iiopp
{

/** @brief C++ wrapper for @ref iio_create_context
 */
ContextPtr create_context(iio_context_params * params, const char * uri)
{
    return ContextPtr{impl::check(iio_create_context(params, uri), "iio_create_context")};
}

ScanPtr scan(struct iio_context_params const * params, char const * backends)
{
    return ScanPtr(impl::check(iio_scan(params, backends), "iio_scan"));
}

ChannelsMaskPtr create_channels_mask(unsigned int nb_channels)
{
    return ChannelsMaskPtr(iio_create_channels_mask(nb_channels));
}

/** @brief Reads the value of a channel by using "input" or "raw" attribute and applying "scale" and "offset" if available
 *
 * @see @c get_channel_value in the example @ref iio-monitor.c
 */
double value(Channel ch)
{
    if (auto att = ch.find_attr("input"))
        return att->read_double() / 1000;

    double scale = 1;
    if (auto att = ch.find_attr("scale"))
        scale = att->read_double();

    double offset = 0;
    if (auto att = ch.find_attr("offset"))
        offset = att->read_double();

    if (auto att = ch.find_attr("raw"))
        return (att->read_double() + offset) * scale / 1000.;

    impl::err(ENOENT, "channel does not provide raw value");
}


}
