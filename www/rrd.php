<?php
    
    ////////////////////////////////////////////////////////////////////////////////////////
    //
    //	humifand 	- 	the humidity fan deamon
    //
    //	Please check www.way2.net for more information
    //
    //  (c) Copyright 2014 by way2.net Services.
    //
    ////////////////////////////////////////////////////////////////////////////////////////
    
    // --- this is the server side of the AJAX graph pages. It is simply an interface to the RRD files
    
    // --- we will give you JSON data
    
    header("content-type: application/json");
    
    // --- this is where our RRD is located
    
    $rrdFile = "/var/local/humifand/humifand.rrd";
    
    // --- build a std option array
    
    $optionArray = array(
                 //        "--step","300",
                         "DEF:itemp="  . $rrdFile . ":itemp:AVERAGE",   //0
                         "DEF:otemp="  . $rrdFile . ":otemp:AVERAGE",   //1
                         "DEF:irh="    . $rrdFile . ":irh:AVERAGE",     //2
                         "DEF:orh="    . $rrdFile . ":orh:AVERAGE",     //3
                         "DEF:idp="    . $rrdFile . ":idp:AVERAGE",     //4
                         "DEF:odp="    . $rrdFile . ":odp:AVERAGE",     //5
                         "DEF:ih="     . $rrdFile . ":ih:AVERAGE",      //6
                         "DEF:oh="     . $rrdFile . ":oh:AVERAGE",      //7
                         "DEF:fanr="   . $rrdFile . ":fanr:LAST",       //8
                         
                         'XPORT:itemp:"itemp"',
                         'XPORT:otemp:"otemp"',
                         'XPORT:irh:"irh"',
                         'XPORT:orh:"orh"',
                         'XPORT:idp:"idp"',
                         'XPORT:odp:"odp"',
                         'XPORT:ih:"ih"',
                         'XPORT:oh:"oh"',
                         'XPORT:fanr:"fanr"');
    
    // --- and add specifics for the timeframe
    
    $tf = $_GET['timeframe'];
    
    switch($tf)
    {
        default:
        case "week":
            array_push($optionArray,"--start");
            array_push($optionArray,"-1w");
            array_push($optionArray,"--step");
            array_push($optionArray,"300");
            break;
            
        case "month":
            array_push($optionArray,"--start");
            array_push($optionArray,"-1m");
            array_push($optionArray,"--step");
            array_push($optionArray,"300");
            break;
            
        case "day":
            array_push($optionArray,"--start");
            array_push($optionArray,"-1d");
            array_push($optionArray,"--step");
            array_push($optionArray,"300");
            break;
    }
    
    // --- fetch all data
    
    $ret_array = rrd_xport( $optionArray );
    
    $hc_array = array();
    
    // --- we got to reformat the array from RRD as there is no "NULL" value in JSON
    
    for ($i = 0; $i < 9; $i++)
    {
        $seriesArray = array();
        
        foreach ($ret_array["data"][$i]["data"] as $key => $value)
        {
            if (is_nan($value))
            {
                $value="NAN";
            }
            else
            {
                $value=round($value,2);
            }
            array_push($seriesArray,array($key*1000,$value));
        }
        
        $hc_array[$i] = $seriesArray;
    }

    // --- we use JSONP just to make our coding life more easy (running th JS code on the dev machine)
    
    echo $_GET['callback']. '('. json_encode($hc_array) . ')';
    
?>