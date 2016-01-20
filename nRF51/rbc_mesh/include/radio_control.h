/***********************************************************************************
Copyright (c) Nordic Semiconductor ASA
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

  3. Neither the name of Nordic Semiconductor ASA nor the names of other
  contributors to this software may be used to endorse or promote products
  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
************************************************************************************/

#ifndef _RADIO_CONTROL_H__
#define _RADIO_CONTROL_H__
#include <stdint.h>
#include <stdbool.h>
/** @brief callbacks for after radio event is complete */
typedef void (*radio_rx_cb)(uint8_t*, bool, uint32_t);
typedef void (*radio_tx_cb)(uint8_t*);

/** @brief callback for when the radio is out of things to do */
typedef void (*radio_idle_cb)(void);

typedef enum
{
    RADIO_EVENT_TYPE_TX,
    RADIO_EVENT_TYPE_RX, /* regular RX. Will be stopped automatically ~100us after start if start time > 0 */
    RADIO_EVENT_TYPE_RX_PREEMPTABLE /* will be aborted when a new event comes in */
} radio_event_type_t;

/**
* @brief executable radio event type 
*/
typedef struct
{
    radio_event_type_t event_type;  /* RX/TX */
    uint8_t* packet_ptr;            /* packet pointer to use. */
    uint8_t access_address;         /* If TX: access address index to send on. If RX: AA enabled bitfield */
    uint8_t channel;                /* Channel to execute event on */
    union 
    {
        radio_rx_cb rx;
        radio_tx_cb tx;
    } callback;
} radio_event_t;

/**
* @brief Starts the radio init procedure
*   Must be called at the beginning of each timeslot
*
* @param[in] access_addr access address to put in aa slot 0
*/
void radio_init(uint32_t access_address, radio_idle_cb idle_callback);

/**
* @brief Schedule a radio event (tx/rx)
* 
* @param[in] radio_event pointer to user-created radio event to be queued. 
*   Is copied into queue, may be stack allocated
*/
bool radio_order(radio_event_t* radio_event);

/**
* @brief Disable the radio. Overrides any ongoing rx or tx procedures
*/
void radio_disable(void);


/**
* @brief Radio event handler, checks any relevant events generated by the radio, and acts accordingly
*/
void radio_event_handler(void);

#endif /* _RADIO_CONTROL_H__*/
