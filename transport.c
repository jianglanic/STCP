/*
 * transport.c 
 *
 * COS461: Assignment 3 (STCP)
 *
 * This file implements the STCP layer that sits between the
 * mysocket and network layers. You are required to fill in the STCP
 * functionality in this file. 
 *
 */


#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <arpa/inet.h>
#include "mysock.h"
#include "stcp_api.h"
#include "transport.h"
#include "mysock_impl.h"


enum {
    CSTATE_CLOSED,
    CSTATE_SYN_SENT,
    CSTATE_ESTABLISHED };    /* you should have more states */


/* this structure is global to a mysocket descriptor */
typedef struct
{
    bool_t done;    /* TRUE once connection is closed */

    int connection_state;   /* state of the connection (established, etc.) */
    tcp_seq initial_sequence_num;
    tcp_seq next_seq;
    tcp_seq ack;
    int slide_window;
    
    char* recv_buffer;//ZX
    char* send_buffer;//LAN
    

    /* any other connection-wide global variables go here */
} context_t;


static void generate_initial_seq_num(context_t *ctx);
static void control_loop(mysocket_t sd, context_t *ctx);
/*
 */
static int transport_recv_head(mysocket_t sd, void* headbuf, int headlen)//LAN
{
    //use ntohl ntohs decode related header
    
    return -1;
}
/*
 */
static int transport_recv_data(mysocket_t sd, void* buf, int headlen)//LAN
{
    
    
    return -1;
}
/*
*/
static void transport_send_data(mysocket_t sd, void* data, int len, context_t* ctx)//LAN
{
    
    //send real app data or stub data
    
}
/*
 */
static void transport_send_head(mysocket_t sd, struct tcphdr * head, int hflag, context_t* ctx)//LAN
{
    
    //use htonl htons to codec
    
}

static bool_t transport_3way_handshake(mysocket_t sd, context_t *ctx)//ZX
{
    
    
    return 0;
}


static bool_t transport_2way_close(mysocket_t sd, context_t *ctx)//ZX
{
    
    return 0;
}

/* initialise the transport layer, and start the main loop, handling
 * any data from the peer or the application.  this function should not
 * return until the connection is closed.
 */
void transport_init(mysocket_t sd, bool_t is_active)
{
    context_t *ctx;

    ctx = (context_t *) calloc(1, sizeof(context_t));
    assert(ctx);
    
    mysock_context_t * sdctx = (mysock_context_t *)stcp_get_context(sd);
 
    generate_initial_seq_num(ctx);
    ctx->connection_state = CSTATE_CLOSED;
    //init all buffer

    /* XXX: you should send a SYN packet here if is_active, or wait for one
     * to arrive if !is_active.  after the handshake completes, unblock the
     * application with stcp_unblock_application(sd).  you may also use
     * this to communicate an error condition back to the application, e.g.
     * if connection fails; to do so, just set errno appropriately (e.g. to
     * ECONNREFUSED, etc.) before calling the function.
     */
    //send SYN if(ctx->is_active)
    if(sdctx->is_active){
        transport_send(sd, NULL, 0, TH_SYN, ctx);
        ctx->connection_state = CSTATE_SYN_SENT;
        
        
        //wait for SYN+ACK
        unsigned int event = stcp_wait_for_event(sd, 0, NULL);
        if(event & NETWORK_DATA)
        {
            //stcp_network_recv
            
            //if it is  SNY + ACK message
        }
        else{
            errno = ECONNREFUSED;
            stcp_unblock_application(sd);
            free(ctx);
            return;
        }
    }
    else{//wait for SYN message
        unsigned int event = stcp_wait_for_event(sd, 0, NULL);
        if(event & NETWORK_DATA)
        {
            //stcp_network_recv
            
            //if it is  SNY + ACK message
        }
        else{
            errno = ECONNREFUSED;
            stcp_unblock_application(sd);
            free(ctx);
            return;
        }
    }
    
    //init send and recv buffer here
    
    
    
    ctx->connection_state = CSTATE_ESTABLISHED;
    stcp_unblock_application(sd);

    control_loop(sd, ctx);

    /* do any cleanup here */
    free(ctx);
}



/* generate random initial sequence number for an STCP connection */
static void generate_initial_seq_num(context_t *ctx)
{
    assert(ctx);

#ifdef FIXED_INITNUM
    /* please don't change this! */
    ctx->initial_sequence_num = 1;
#else
    ctx->initial_sequence_num = rand()%255;
    /* you have to fill this up */
    /*ctx->initial_sequence_num =;*/
#endif
}


/* control_loop() is the main STCP loop; it repeatedly waits for one of the
 * following to happen:
 *   - incoming data from the peer
 *   - new data from the application (via mywrite())
 *   - the socket to be closed (via myclose())
 *   - a timeout
 */
static void control_loop(mysocket_t sd, context_t *ctx)
{
    assert(ctx);
    assert(!ctx->done);

    while (!ctx->done)
    {
        unsigned int event;

        /* see stcp_api.h or stcp_api.c for details of this function */
        /* XXX: you will need to change some of these arguments! */
        event = stcp_wait_for_event(sd, 0, NULL);

        /* check whether it was the network, app, or a close request */
        if (event & APP_DATA)//LAN
        {
            /* the application has requested that data be sent */
            /* see stcp_app_recv() */
            //write to send buffer
            
            //buff is full send stcp_network_send();
        }
        
        if(event & NETWORK_DATA)//ZX
        {
            //stcp_network_recv();
            //write recv buffer
            
            //recv_window = TH_WIN
            
            //generate local free recv buf;
            
            //send ACK to peer
            
            //if(buff is full), stcp_app_send; stcp_unblock_application(sd);
        
        }
        
        if(event & TIMEOUT)//ZX
        {
            //buff is full send stcp_network_send();
            
        }

        if(event & APP_CLOSE_REQUESTED)//ZX
        {
            
            if(!transport_2way_close(sd, ctx))
            {
                errno = 0;//FIXME
            }
        }
        
        
        
        /* etc. */
    }
}


/**********************************************************************/
/* our_dprintf
 *
 * Send a formatted message to stdout.
 * 
 * format               A printf-style format string.
 *
 * This function is equivalent to a printf, but may be
 * changed to log errors to a file if desired.
 *
 * Calls to this function are generated by the dprintf amd
 * dperror macros in transport.h
 */
void our_dprintf(const char *format,...)
{
    va_list argptr;
    char buffer[1024];

    assert(format);
    va_start(argptr, format);
    vsnprintf(buffer, sizeof(buffer), format, argptr);
    va_end(argptr);
    fputs(buffer, stdout);
    fflush(stdout);
}


