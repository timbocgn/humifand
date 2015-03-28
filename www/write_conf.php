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
    
    // --- this is the server side of the AJAX config pages. It is simply an interface to the config files (write part)
    
    
    $categories = json_decode(file_get_contents('php://stdin'));
    
    
    // --- we will give you JSON data
    
    //header("content-type: application/json");
    
    // --- this is where our conf file is located
    
    $confFile = "/etc/humifand.conf";
    
    // --- open the file
    
    $myfile = fopen($confFile, "r") or die("0Unable to open config file!");
    
    // --- read line by line
/*
    $hc_array = array();
    
    while(!feof($myfile))
    {
        // --- read line and remove tailing whitespaces
        
        $myline = trim(fgets($myfile));
        
        // --- remove comments
        
        if (strpos($myline, "#") !== FALSE)
            $myline = trim(stristr ($myline,"#",true));
        
        if (strlen($myline) > 0)
        {
            // --- split var and value
            
            $tokens = explode("=",$myline);
            
            if (count($tokens) > 0)
            {
                //echo "Key:" . $tokens[0] . "\n";
                //echo "Value:" . $tokens[1] . "\n";
                
                $hc_array[trim($tokens[0])] = trim($tokens[1]);
                
            }
        }
        
    }*/
    
    
    fclose($myfile);
    
    // --- we use JSONP just to make our coding life more easy (running th JS code on the dev machine)
    
    echo "1";
    
    ?>