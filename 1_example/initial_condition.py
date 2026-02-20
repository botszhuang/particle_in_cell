import numpy as np
import os

path="./input/"
gridFile = path+"grid.tex"
particleFile = path+"particle.tex"

xboundarys = [ 0, 10 ]
yboundarys = [ 0, 10 ]
vMax = 0.5

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

#initialize the particles position
xWidth = xboundarys[1] - xboundarys[0]
yWidth = yboundarys[1] - yboundarys[0]

if os.path.exists( particleFile ):
    os.system( "rm -f %s " % ( particleFile ) )

with open( particleFile , "w" ) as f:
    f.write( "# x y vx vy\n" )
    for i in range( 1000 ):

        #position
        x = np.random.rand() * xWidth + xboundarys[0]
        y = np.random.rand() * yWidth + yboundarys[0]

        #velocity
        vx = np.random.rand() * 2 * vMax - vMax
        vy = np.random.rand() * 2 * vMax - vMax

        # output
        f.write( "%.6f %.6f %.6f %.6f\n" % ( x, y, vx, vy ) )