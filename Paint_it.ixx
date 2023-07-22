export module M_Paint;

export char paint_val(double ballc, double balla, double blockc, double blocka)
{
	double sc = ballc / 255;//source
	double sa = balla / 255;
	double dc = blockc / 255;//destination
	double da = blocka / 255;

	double color = sc * sa + dc * da * (1 - sa);
	return color * 255;
}