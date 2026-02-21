import numpy as np
import os

path="./input/"
gridFile = path+"grid.tex"
particle_XFile = path+"particle_position.tex"
particle_VFile = path+"particle_velocity.tex"

xboundarys = [ 0, 3 ]
yboundarys = [ 0, 3 ]
vMax = 0.5
particle_number = 100

#initialize the input directory
if not os.path.exists( path ):
    os.makedirs( path )

#initialize the grid
if os.path.exists( gridFile ):
    os.system( "rm -f %s " % ( gridFile ) )

with open( gridFile , "w" ) as f:
    f.write( "# x y\n" )
    for x in range( xboundarys[0], xboundarys[1] ):
        for y in range( yboundarys[0], yboundarys[1] ):
            f.write( "%d %d\n" % ( x, y ) )

#initialize the particles position and velocity
xWidth = xboundarys[1] - xboundarys[0]
yWidth = yboundarys[1] - yboundarys[0]

if os.path.exists( particle_XFile ):
    os.system( "rm -f %s " % ( particle_XFile ) )

if os.path.exists( particle_VFile ):
    os.system( "rm -f %s " % ( particle_VFile ) )    

with open( particle_XFile , "w" ) as X:
    with open( particle_VFile , "w" ) as V:
        X.write( "# x y\n" )
        V.write( "# vx vy\n" )    
        
        for i in range( particle_number ):

            #position
            x = np.random.rand() * xWidth + xboundarys[0]
            y = np.random.rand() * yWidth + yboundarys[0]
            X.write( "%.6f %.6f\n" % ( x, y) )

            #velocity
            vx = np.random.rand() * 2 * vMax - vMax
            vy = np.random.rand() * 2 * vMax - vMax
            V.write( "%.6f %.6f\n" % ( vx, vy ) )


     