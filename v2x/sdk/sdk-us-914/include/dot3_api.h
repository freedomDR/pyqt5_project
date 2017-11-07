/* Copyright (C) Industrial Technology Research Institute - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Po-Chun, Kang <ttt710516@itri.org.tw>
 */

#ifndef	__DOT3_API_H__
#define	__DOT3_API_H__

/* Management Plane APIs */

/**
 * @brief  the function init...
 *
 * @param handle oao ao 
 * @return if it is ok
 */
int wme_init(wme_handle_t *handle);

int wme_exit(wme_handle_t *handle);

int wme_provider_serv_create(wme_handle_t *handle, unsigned short *serv_index, wme_provider_info_t *info);

int wme_provider_serv_change(wme_handle_t *handle, unsigned short serv_index, wme_provider_info_t *info);

int wme_provider_serv_delete(wme_handle_t *handle, unsigned short serv_index);

int wme_user_serv_create(wme_handle_t *handle, unsigned short *serv_index, wme_user_info_t *info);

int wme_user_serv_delete(wme_handle_t *handle, unsigned short serv_index);

int wme_channel_serv_create(wme_handle_t *handle, unsigned short *serv_index, wme_channel_info_t *info);

int wme_channel_serv_delete(wme_handle_t *handle, unsigned short serv_index);

int wme_wsm_serv_create(wme_handle_t *handle, unsigned short *serv_index, wme_wsm_info_t *info);

int wme_wsm_serv_delete(wme_handle_t *handle, unsigned short serv_index);
/**
 * @brief The function is for register/unregister the provider service.
 *
 * @param handle The pointer of handle 
 * @return If success return 0, otherwise return nagtive value.
 */
int wme_provider_service(wme_handle_t *handle, struct provider_info *info);

int wme_user_service(wme_handle_t *handle, struct user_info *info);

int wme_channel_service(wme_handle_t *handle, struct channel_info *info);

int wme_wsm_service(wme_handle_t *handle, struct wsm_info *info);

//int wme_device_set(unsigned char dev_id, struct dev_info *info);

int wme_wsa_cfg_get(struct wsa_cfg_info *info);

int wme_wsa_cfg_set(struct wsa_cfg_info *info);

int wme_dev_info_get(unsigned char dev_id, struct dev_info *info);

int wme_mib_get(struct mib_info *info);

int wme_event_recv(wme_handle_t *handle, struct event_message *event, unsigned int timeout);

int wme_event_recv_notify(wme_handle_t *handle, void (*cb_func)(wme_handle_t *handle));

int wme_event_waiting_terminate(wme_handle_t *handle);


/* Data Plane APIs */

int wme_wsm_send(wme_handle_t *handle, struct out_wsm *wsm);

int wme_wsm_recv(wme_handle_t *handle, struct in_wsm *wsm, unsigned int timeout);

int wme_wsm_recv_notify(wme_handle_t *handle, void (*cb_func)(wme_handle_t *handle));

#endif	/* __DOT3_API_H__ */

