#ifndef _GPS_H_
#define _GPS_H_

#include <time.h>

#define	GPGSA_NO_FIX	'1'
#define	GPGSA_2D_FIX	'2'
#define	GPGSA_3D_FIX	'3'

typedef int32_t gps_ret_t;
#define _GPS_RET(code) ((gps_ret_t)(code))

#define GPS_OK 		_GPS_RET(0)		/* GPS OK */
#define GPS_ERR  	_GPS_RET(-1)	/* GPS error */

struct gps_info {
	int			tp_level;

	int			time_sync;

	unsigned long	tp_ext_jiffies;	// last pulse of external time pluse
	uint32_t	tp_count;
	uint32_t	tp_count_int;
	uint32_t	tp_count_ext;
	int			tp_adjust;
	long		tp_delta;		// in usec

	uint32_t	time_0msec_count;
	uint32_t	time_adjust;

	// nmea buffer
	uint32_t	nmea_lines;
	char		nmea_buf[128];

	uint32_t	nmea_lines_RMC;
	uint32_t	nmea_lines_GGA;
	uint32_t	nmea_lines_GSA;
	uint32_t	nmea_lines_GST;

	uint32_t	drop_lines_RMC;
	uint32_t	drop_lines_GGA;

	// GPS fixed info
	char	status;			// $GPRMC: 2
	char	date[12];		// $GPRMC: 9 (UTC YYYY-MM-DD)
	char	time_RMC[20];	// $GPRMC: 1 (UTC hh:mm:ss[.ms])
	char	time_GGA[20];	// $GPGGA: 1 (UTC hh:mm:ss[.ms])
//	char	*time;			// $GPRMC: 1	|| $GPGGA: 1 	(UTC hh:mm:ss[.ms])
	time_t  time;
//	char	latitude[16];	// $GPRMC: 3, 4	|| $GPGGA: 2, 3
//	char	longitude[16];	// $GPRMC: 5, 6	|| $GPGGA: 4, 5
	double	latitude;
	double  longitude;
//	char	knots[8];		// $GPRMC: 7
	double 	groundspeed;
//	char	angle[8];		// $GPRMC: 8
	double  angle;

	char	quality[4];		// $GPGGA: 6
	char	satellites[4];	// $GPGGA: 7
//	char	altitude[16];	// $GPGGA: 9, 10
	double  altitude;

	char	ellipsoid_h[16];	// $GPGGA: 11, 12

	char	gsa_3d_fix;		// $GPGSA: 2

/*	char	gst_semi_major[8];	// $GPGST: 3
	char	gst_semi_minor[8];	// $GPGST: 4
	char	gst_semi_orient[8];	// $GPGST: 5*/

	/* define in SDK/include/atlk/nav.h */
	double gst_semi_major;  // $GPGST: 3, error_position_horizontal_semi_major_axis_length_m
	double gst_semi_minor;	// $GPGST: 4, error_position_horizontal_semi_minor_axis_length_m
	double gst_semi_orient;	// $GPGST: 5, error_position_horizontal_major_axis_direction_deg*/

};

typedef struct gps_info_plus {
	struct gps_info gps;
	int ms;
	double acc_100ms;
	double acc_100ms_lat;
	double acc_100ms_lon;
	double yaw_rate;

	double heading_100ms;
	double heading_500ms;
	double heading_1000ms;
} gps_info_p;

void gps_init(void);
void gps_exit(void);
gps_ret_t get_gps_info(struct gps_info * gps_ret);
gps_ret_t get_gps_info_list(gps_info_p * gps_ret);

#endif

