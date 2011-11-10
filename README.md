Overview
------------
This is a geoip client and server that runs under linux. And while this code is under the MIT license, the data files required by this program require a license from Maxmind.com.

Please visit their website to obtain the city-level geoip CSVs. 


Dependencies
------------

  - MaxMind.com City Data - http://www.maxmind.com/app/city
  - Boost 1.37 (apt-get install libboost-all-dev)
  - cmake 2.8.2 (apt-get install cmake)


Installation
------------

  - git clone git://github.com/homer6/geoipd.git
  - cd geoipd
  - cmake .
  - make

  
Usage
-----
Starting the server (the server will only listen locally):
  ./geoipd --locations-file=GeoIPCity-134-Location.csv --blocks-file=GeoIPCity-134-Blocks.csv

Running the client, once the server is running:
  ./geoip --host=localhost --ip-address=206.87.31.18
  {"city":"Kelowna","region":"BC","country":"CA","latitude":"49.9000","longitude":"-119.4833","area_code":"","metro_code":"","postal_code":""}


Possible Results
----------------
Normal Result (only country is guaranteed):
  {"city":"Kelowna","region":"BC","country":"CA","latitude":"49.9000","longitude":"-119.4833","area_code":"","metro_code":"","postal_code":""}

No result found:
  {}

Error (if the server is down):
  {"error":"Connection Refused"}

