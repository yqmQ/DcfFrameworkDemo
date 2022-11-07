#include"dcf_interface.h"
#include"dcf_setparams.h"

bool Set_dcf_param()
{
    if (dcf_set_param("ELECTION_TIMEOUT", "100") != 0)
    {
        return false;
    }

    if (dcf_set_param("DATA_PATH", "../primary_data_dir/dcf_data") != 0)
    {

        return false;
    }

    if (dcf_set_param("LOG_PATH", "../primary_data_dir/dcf_log") != 0)
    {

        return false;
    }

    if (dcf_set_param("INSTANCE_NAME", "DcfFrameworkDemo") != 0)
    {

        return false;
    }


    return true;
}