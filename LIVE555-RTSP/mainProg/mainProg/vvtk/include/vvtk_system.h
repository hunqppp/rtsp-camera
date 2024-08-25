#ifndef __VVTK_SYSTEM_H__
#define __VVTK_SYSTEM_H__
#ifdef __cplusplus
extern "C"
{
#endif
#include "vvtk_def.h"
#define MAX_SN_LENGTH 128

    /**
     * @brief Enumeration of VVTK network interfaces.
     *
     * This enumeration defines the different network interfaces supported by the VVTK system,
     * including Ethernet (wired), Wi-Fi station (client), and Wi-Fi access point (AP) modes.
     */
    typedef enum
    {
        VVTK_NETIF_ETH = 0,
        VVTK_NETIF_WIFI_STA,
        VVTK_NETIF_WIFI_AP,
    } VVTK_NETIF;

    /**
     * @brief Structure representing network information.
     *
     * This structure contains information about the network configuration including IP address,
     * subnet mask, primary and secondary DNS servers, and gateway address.
     */
    typedef struct
    {
        char ip[16];
        char submask[16];
        char dns1[64];
        char dns2[64];
        char gw[16];
    } VVTK_NET_INFO;

	typedef struct 
	{
		int eth_connectivity;
		int wlan_connectivity;
	} VVTK_NET_CONNECTIVITY;
    /**
     * @brief Type definition for a callback function used in QR code scanning.
     *
     * This callback function is invoked when a QR code is scanned. It receives the payload
     * of the QR code and its length as parameters.
     *
     * @param payload A pointer to the payload data of the scanned QR code.
     * @param payload_len The length of the payload data.
     */
    typedef void (*VVTK_QRCODE_CALLBACK)(char *payload, int patload_len);

	/**
     * @brief Type definition for a callback function used in network monitor.
     *
     * This callback function is invoked when network has disconnected. It receives the
	 * VVTK_NET_CONNECTIVITY to show the network status.
     *
     * @param net_status A pointer to VVTK_NET_CONNECTIVITY.
     */
    typedef void (*VVTK_NET_MONITOR_CALLBACK)(VVTK_NET_CONNECTIVITY *net_status);

    /**
     * @brief Initiates a system reboot.
     *
     * This function is responsible for triggering a system reboot using the VVTK framework.
     * It performs the necessary operations to gracefully restart the system.
     *
     * @return VVTK_RET value indicating the success or failure of the reboot operation.
     *         Use this return value to handle errors or confirm successful reboot.
     */

    VVTK_RET vvtk_system_reboot();
    /**
     * @brief Perform firmware Over-The-Air (OTA) update for the system.
     *
     * This function initiates an Over-The-Air update for the system firmware using the specified
     * file path. The file at the given path should contain the updated firmware image.
     *
     * @param filepath The path to the firmware image file for OTA update.
     * @return VVTK_RET_SUCCESS if the OTA update is successful, error code otherwise.
     */
    VVTK_RET vvtk_system_fw_ota(char *filepath);

    /**
     * @brief Perform application Over-The-Air (OTA) update for the system.
     *
     * This function initiates an Over-The-Air update for the system application using the specified
     * file path. The file at the given path should contain the updated application image.
     *
     * @param filepath The path to the application image file for OTA update.
     * @return VVTK_RET_SUCCESS if the OTA update is successful, error code otherwise.
     */
    VVTK_RET vvtk_system_app_ota(char *filepath);

    /**
     * @brief Get the serial number of the VVTK system.
     *
     * This function retrieves the serial number of the VVTK system.
     * The serial number string will be null-terminated and truncated if the buffer size is insufficient.
     *
     * @param sn A pointer to the buffer where the serial number string will be stored.
     * @param length The length of the buffer pointed to by 'sn'.
     * @return VVTK_RET_SUCCESS if the serial number is successfully retrieved, error code otherwise.
     */
    VVTK_RET vvtk_system_get_sn(char *sn, int len);

    /**
     * @brief Get the Etherent MAC address of the VVTK system.
     *
     * This function retrieves the MAC address of the VVTK system and stores it in the provided buffer.
     * The MAC address string will be null-terminated and truncated if the buffer size is insufficient.
     *
     * @param macaddr A pointer to the buffer where the MAC address string will be stored.
     * @param len The length of the buffer pointed to by 'macaddr'.
     * @return VVTK_RET_SUCCESS if the MAC address is successfully retrieved, error code otherwise.
     */
    VVTK_RET vvtk_system_get_mac(char *macaddr, int len);

    /**
     * @brief Get the Wi-Fi MAC address of the VVTK system.
     *
     * This function retrieves the Wi-Fi MAC address of the VVTK system and stores it in the provided buffer.
     * The MAC address string will be null-terminated and truncated if the buffer size is insufficient.
     *
     * @param macaddr A pointer to the buffer where the Wi-Fi MAC address string will be stored.
     * @param len The length of the buffer pointed to by 'macaddr'.
     * @return VVTK_RET_SUCCESS if the Wi-Fi MAC address is successfully retrieved, error code otherwise.
     */
    VVTK_RET vvtk_system_get_wifi_mac(char *macaddr, int len);
    /**
     * @brief Get hardware information of the VVTK system.
     *
     * This function retrieves hardware information of the VVTK system and stores it in the provided buffer.
     * The hardware information is formatted in JSON style and will be null-terminated.
     * If the buffer size is insufficient to hold the entire JSON string, it will be truncated.
     *
     * @param hwinfo A pointer to the buffer where the hardware information JSON string will be stored.
     * @param len The length of the buffer pointed to by 'hwinfo'.
     * @return VVTK_RET_SUCCESS if the hardware information is successfully retrieved, error code otherwise.
     */
    VVTK_RET vvtk_system_get_hw_info(char *hwinfo, int len);
    /**
     * @brief Start network configuration for the VVTK system.
     *
     * This function starts network configuration for the VVTK system using the specified network interface.
     * It initiates DHCP client to obtain an IP address from the DHCP server.
     *
     * @param netif The network interface to be used for network configuration.
     * @return VVTK_RET_SUCCESS if the network configuration is successfully initiated, error code otherwise.
     */
    VVTK_RET vvtk_system_start_network(VVTK_NETIF netif);

    /**
     * @brief Get the IP address of the VVTK system on the specified network interface.
     *
     * This function retrieves the IP address of the VVTK system on the specified network interface
     * and stores it in the provided buffer. The IP address string will be null-terminated.
     * If the buffer size is insufficient to hold the entire IP address string, it will be truncated.
     *
     * @param netif The network interface for which the IP address is requested.
     * @param ip A pointer to the buffer where the IP address string will be stored.
     * @param len The length of the buffer pointed to by 'ip'.
     * @return VVTK_RET_SUCCESS if the IP address is successfully retrieved, error code otherwise.
     */
    VVTK_RET vvtk_system_get_network_ip(VVTK_NETIF netif, char *ip, int len);

    /**
     * @brief Get network information of the VVTK system on the specified network interface.
     *
     * This function retrieves network information of the VVTK system on the specified network interface
     * and stores it in the provided structure. The structure contains IP address, subnet mask,
     * primary and secondary DNS servers, and gateway information.
     *
     * @param netif The network interface for which the network information is requested.
     * @param info A pointer to the structure where the network information will be stored.
     * @return VVTK_RET_SUCCESS if the network information is successfully retrieved, error code otherwise.
     */
    VVTK_RET vvtk_system_get_network_info(VVTK_NETIF netif, VVTK_NET_INFO *info);

    /**
     * @brief Set RTSP (Real-Time Streaming Protocol) account credentials for the VVTK system.
     *
     * This function sets the RTSP account credentials (username and password) for the VVTK system
     * to be used for authentication during RTSP streaming.
     *
     * @param account A pointer to the null-terminated string containing the RTSP account username.
     * @param password A pointer to the null-terminated string containing the RTSP account password.
     * @return VVTK_RET_SUCCESS if the RTSP account credentials are successfully set, error code otherwise.
     */
    VVTK_RET vvtk_system_set_rtsp_account(char *account, char *password);

    /**
     * @brief Get the capability information of the VVTK system.
     *
     * This function retrieves the capability information of the VVTK system and stores it in the provided buffer.
     * The capability information is formatted in JSON style and will be null-terminated.
     * If the buffer size is insufficient to hold the entire JSON string, it will be truncated.
     *
     * @param json_capability A pointer to the buffer where the capability information JSON string will be stored.
     * @param length The length of the buffer pointed to by 'json_capability'.
     * @return VVTK_RET_SUCCESS if the capability information is successfully retrieved, error code otherwise.
     */
    VVTK_RET vvtk_system_get_capability(char *json_capability, int length);

    /**
     * @brief Get the version of the VVTK library.
     *
     * This function retrieves the version of the VVTK library and stores it in the provided buffer.
     * The version string will be null-terminated and truncated if the buffer size is insufficient.
     *
     * @param version A pointer to the buffer where the version string will be stored.
     * @param length The length of the buffer pointed to by 'version'.
     * @return VVTK_RET_SUCCESS if the version is successfully retrieved, error code otherwise.
     */
    VVTK_RET vvtk_lib_version(char *version, int length);

    /**
     * @brief Start scanning for QR codes.
     *
     * This function initiates the scanning process for QR codes. It takes a callback function
     * as a parameter, which will be called when a QR code is successfully scanned.
     *
     * @param cb The callback function to be called when a QR code is scanned.
     * @return VVTK_RET_SUCCESS if the scanning process is successfully initiated, error code otherwise.
     */
    VVTK_RET vvtk_qrcode_scan_start(VVTK_QRCODE_CALLBACK cb);

    /**
     * @brief Stop scanning for QR codes.
     *
     * This function stops the scanning process for QR codes.
     *
     * @return VVTK_RET_SUCCESS if the scanning process is successfully stopped, error code otherwise.
     */
    VVTK_RET vvtk_qrcode_scan_stop();

    /**
     * @brief Set uboot password.
     *
     * This function allow to set uboot password . Default password is 'a'. Max length of password is 32.
     *
     * @param passwd A pointer to the null-terminated string containing the uboot password, set NULL to reset to default password 'a'.
     * @return VVTK_RET_SUCCESS if the password is set successfully, error code otherwise.
     */
    VVTK_RET vvtk_system_set_uboot_passwd(char *passwd);

    /**
     * @brief Set root login password.
     *
     * This function allow to set root login password . Default password is none, login with root account with 'enter' key.
     * Max length of password is 32.
     *
     * @param passwd A pointer to the null-terminated string containing the root password.
     * @return VVTK_RET_SUCCESS if the password is set successfully, error code otherwise.
     */
    VVTK_RET vvtk_system_set_root_passwd(char *passwd);
	/**
     * @brief Start/stop monitoring to network connectivity.
     *
     * This function starts/stops a thread to continuously monitor the connection of network and invloke 
	 * the callback function if it discconnected.
	 * 
     *
     * @param cb The callback function to be called if the disconnection occurred.
     * @return VVTK_RET_SUCCESS if the monitor process is successfully initiated, error code otherwise.
     */
   	VVTK_RET vvtk_system_set_network_monitor(int enable, VVTK_NET_MONITOR_CALLBACK cb);

	/**
     * @brief Check the network monitor thread is running or not.
     *
     * This fuction get the status of monitor thread.
	 * 
     * @param enable The pointer of an interger shows the existence of the monitor thread.
     * @return VVTK_RET_SUCCESS if it is successful to get the status, error code otherwise.
     */
	VVTK_RET vvtk_system_get_network_monitor(int *enable);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __VVTK_SYSTEM_H__ */
