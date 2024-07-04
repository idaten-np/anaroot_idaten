



#define NCH 48



class gamma_flight_time
{
	public:
		gamma_flight_time();
		~gamma_flight_time();

		double flight_distance (int idet, double source_pos_x, double source_pos_y, double source_pos_z=0.);
		double flight_time (int idet, double source_pos_x, double source_pos_y, double source_pos_z=0.);


	private: 
		const double distance = 170.0 + 1.5/2*2.54;
		const double theta[NCH] = {40 ,	40 ,	40 ,	40 ,	60 ,	60 ,	60 ,	60 ,	60 ,	80 ,	80 ,	80 ,	100 ,	100 ,	100 ,	100 ,	120 ,	120 ,	120 ,	120 ,	140 ,	140 ,	140 ,	140 ,	40 ,	40 ,	40 ,	40 ,	60 ,	60 ,	60 ,	60 ,	80 ,	80 ,	80 ,	80 ,	100 ,	100 ,	100 ,	120 ,	120 ,	120 ,	120 ,	120 ,	140 ,	140 ,	140 ,	140 };
		const double phi[NCH] = {70 ,	30 ,	-10 ,	-50 ,	90 ,	50 ,	10 ,	-30 ,	-70 ,	70 ,	50 ,	-50 ,	90 ,	50 ,	-30 ,	-70 ,	70 ,	30 ,	-10 ,	-50 ,	90 ,	50 ,	10 ,	-30 ,	110 ,	150 ,	-170 ,	-130 ,	130 ,	170 ,	-150 ,	-100 ,	110 ,	150 ,	-130 ,	-90 ,	130 ,	-150 ,	-110 ,	110 ,	150 ,	-170 ,	-130 ,	-90 ,	130 ,	170 ,	-150 ,	-110 };
		double det_pos_x[NCH]={0};
		double det_pos_y[NCH]={0};
		double det_pos_z[NCH]={0};




};


gamma_flight_time::gamma_flight_time()
{
	for (int i=0; i<NCH; i++)
	{
		det_pos_x[i] = sin(theta[i]/180.)*cos(phi[i]/180.)*distance;
		det_pos_y[i] = sin(theta[i]/180.)*sin(phi[i]/180.)*distance;
		det_pos_z[i] = cos(theta[i]/180.)*distance;
	}
}
gamma_flight_time::~gamma_flight_time()
{
}

double gamma_flight_time::flight_distance(int idet, double source_pos_x, double source_pos_y, double source_pos_z)
{
	if (idet<0 || idet>=NCH)
	{
		fprintf(stderr,"gamma_flight_time -> (idet<0 || idet>=NCH)\n");
		return 0;
	}
	double dx = det_pos_x[idet]-source_pos_x;
	double dy = det_pos_y[idet]-source_pos_y;
	double dz = det_pos_z[idet]-source_pos_z;
	return sqrt(dx*dx+dy*dy+dz*dz);
}


double gamma_flight_time::flight_time(int idet, double source_pos_x, double source_pos_y, double source_pos_z)
{
	//double c = 2.99792458e8; // m/s
	//double c = 2.99792458e-1; // m/ns
	//double c = 2.99792458e2; // mm/ns
	double c = 2.99792458e-1; // mm/ps
	return flight_distance(idet, source_pos_x, source_pos_y, source_pos_z) / c;
}



