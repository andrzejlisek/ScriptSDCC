// Gets status of other script from bundle
// N - Bundle item number
// Returns: 0 - Idle
//          1 - Running
//          2 - Run not possible
uchar bundle_status(uchar N)
{
 _uchar[mem_swap + 1] = N;
 _uchar[mem_swap] = 246;
 return _uchar[mem_swap + 1];
}

// Runs other script from bundle
// N - Bundle item number
// Returns: 0 - Script not ran
//          1 - Script ran
uchar bundle_run(uchar N)
{
 _uchar[mem_swap + 1] = N;
 _uchar[mem_swap] = 247;
 return _uchar[mem_swap + 1];
}
