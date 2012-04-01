#!/usr/bin/python
# pimcplot.py
# Adrian Del Maestro
# 07.20.2009
# 
# Plot rough estimators vs. MC Bins for files supplied as input

import matplotlib
matplotlib.use('TKAgg')

import os,sys
import pyutils
import loadgmt,kevent
from pylab import *
import argparse
import pimchelp


# Begin Main Program 
# -----------------------------------------------------------------------------
def main(): 

    # setup the command line parser options 
    parser = argparse.ArgumentParser(description='Plot Raw MC Equilibration Data for Scalar Estimators.')
    parser.add_argument('fileNames', help='Scalar estimator files', nargs='+')
    parser.add_argument('--estimator','-e', help='A list of estimator names that \
                        are to be plotted.', type=str)
    parser.add_argument('--skip','-s', help='Number of measurements to be skipped \
                        in the average plot.', type=int, default=0)
    args = parser.parse_args()

    fileNames = args.fileNames

    if len(fileNames) < 1:
        parser.error("Need to specify at least one scalar estimator file")

    # We count the number of lines in the estimator file to make sure we have
    # some data and grab the headers
    headers = pimchelp.getHeadersDict(fileNames[0])

    # If we don't choose an estimator, provide a list of possible ones
    if not args.estimator:
        errorString = "Need to specify one of:\n"
        for head,index in headers.iteritems():
            errorString += "\"%s\"" % head + "   "
        parser.error(errorString)

    numFiles = len(fileNames)
    col = list([headers[args.estimator]])

    label = pimchelp.Description()
    yLong = label.estimatorLongName[args.estimator]
    yShort = label.estimatorShortName[args.estimator]

    # ============================================================================
    # Figure 1 : column vs. MC Steps
    # ============================================================================
    figure(1)
    connect('key_press_event',kevent.press)

    colors  = loadgmt.getColorList('cw/1','cw1-029',max(numFiles,2))

    n = 0
    for fileName in fileNames:

        dataFile = open(fileName,'r');
        dataLines = dataFile.readlines();
        dataFile.close()

        if len(dataLines) > 2:
            data = loadtxt(fileName,usecols=col)
            plot(data,marker='s',color=colors[n],markeredgecolor=colors[n],\
                        markersize=4,linestyle='-',linewidth=1.0)
    
            n += 1

    ylabel(yLong)
    xlabel("MC Bin Number")

    # ============================================================================
    # Figure 2 : running average of column vs. MC Bins
    # ============================================================================
    figure(2)
    connect('key_press_event',kevent.press)

    n = 0
    for fileName in fileNames:

        dataFile = open(fileName,'r');
        dataLines = dataFile.readlines();

        if len(dataLines) > 2:

            data = loadtxt(fileName,usecols=col)
            if size(data) > 1:
                aveData = []
                for r in range(args.skip+1,len(data)+1):
                    aveData.append(mean(data[args.skip:r]))

                plot(aveData,color=colors[n],linewidth=1.5,marker='None',linestyle='-')
    
                n += 1

    ylabel(r'$\langle$' + yShort + r'$\rangle$')
    xlabel("MC Bin Number")
    tight_layout()

    show()
# ----------------------------------------------------------------------
# ----------------------------------------------------------------------
if __name__ == "__main__": 
    main()
