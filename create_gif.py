#!/usr/bin/python

###########################################################################
#
# name          : create_gif.py
#
# purpose       : copy glitched images to folder for use in creating gif
#
# usage         : python create_gif.py
#
# desciption    : file fuzzer produces corrupted images in numbered folders
#                 agnostic to the intended set (multiple source files could
#                 be intended to aggregate)
#
###########################################################################

import hashlib
import os
import shutil

root_dir = "<output directory>"
output_dir = os.path.join( root_dir, "output" )
target_dir = os.path.join( root_dir, "final" )

if __name__ == "__main__" :

    start_dir = 5
    end_dir = 8
    token_id = "03"

    input_dirs = [ os.path.join( output_dir, str( i ) ) for i in range( start_dir, end_dir ) ]
    dest_dir = os.path.join( target_dir, token_id )
    
    if not os.path.exists( dest_dir ) :
        os.makedirs( dest_dir )

    for d in input_dirs :
        filenames = [ os.path.join( d, i ) for i in os.listdir( d ) ]
        for ff in filenames :
            readable_hash = None
            with open( ff, 'rb' ) as f :
                b = f.read()
                readable_hash = hashlib.sha256( b ).hexdigest()
                print( readable_hash )

            if readable_hash is not None :
                dest_filename = os.path.join( dest_dir, "%s.bmp" % readable_hash )
                shutil.copy( ff, dest_filename )
                
    os.system( f"convert -loop 0 -delay 10 {dest_dir}/*bmp {target_dir}/{token_id}.gif 2> /dev/null" )
