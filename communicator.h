/**
 * @file communicator.h
 * @author Adrian Del Maestro
 * @date 03.25.2009	
 *
 * @brief Class definitions for all file input/output
 */

#ifndef COMMUNICATOR_H 
#define COMMUNICATOR_H

#include "common.h"
#include "constants.h"
#include <cstring>
#include <fstream>

// ========================================================================  
// File Class
// ========================================================================  
/** 
 * A basic input/output file class.
 *
 */
class File
{
    public:
        File(string, string, string, string);
        File(string);
        ~File() {close();}

        /* Return the file stream */
		fstream & stream() { 
            return rwfile;
        }
        
		void open(ios_base::openmode);
        void reset();
        void rename();

    protected:
        string name;        // The File name
        string bakname;     // The backup name

        fstream rwfile;     // The i/o file object

        void close();
		void open(ios_base::openmode,string);
};

// ========================================================================  
// Communicator Class
// ========================================================================  
/** 
 * Performs input/output.
 *
 * Holds information on input and output files that will need to be accessed 
 * throughout the simulation and allows their access via the singleton 
 * design pattern.
 * @see http://en.wikipedia.org/wiki/Singleton_pattern
 */
class CCommunicator
{
	public:
		static CCommunicator* getInstance();

		/** Initialize the output files */
		void init(double,bool,string,string);

        /** Get method returning file object */
        File * file(string type) {
            if (!file_.count(type))
                initFile(type);
            return file_[type];
        }
        
	protected:
		CCommunicator() {}									
		CCommunicator(const CCommunicator&);				///< Copy constructor
		CCommunicator& operator= (const CCommunicator&);	///< Singleton equals

	private:
        ios_base::openmode mode;    // The file i/o mode

		string ensemble;			// The type of ensemble
		string dataName;			// The labelling scheme of the output files
        string header;              // A unique file header

        string initName;      // A possible initial file name
        string fixedName;     // A posible fixed file name
        string baseDir;       // The output base directory

		map <string,File*> file_;	// The file map

        /* Makes sure we have a unique PIMCID */
        void getUniqueID(const double);

        /* Initialize a input/output file */
        void initFile(string);
};

// ========================================================================  
// Communicator Class
// ========================================================================  
/** 
 * All files used for input/output.
 *
 * Holds information on input and output files that will need to be accessed 
 * throughout the simulation and allows their access via the singleton 
 * design pattern.
 * @see http://en.wikipedia.org/wiki/Singleton_pattern
 */
class Communicator
{
	public:
		static Communicator* getInstance();

		/* Initialize the output files */
		void init(const double, const bool,const string, const string);

		/* Get methods */
		fstream & logFile() {return logFile_;}				///< Get log file
		fstream & estimatorFile() {return estimatorFile_;}	///< Get estimator file
		fstream & superFile() {return superFile_;}			///< Get superfluid file
		fstream & stateFile() {return stateFile_;}			///< Get state file
		fstream & initFile() {return initFile_;}			///< Get initialization file
		fstream & fixedFile() {return fixedFile_;}			///< Get fixed file
		fstream & wlFile() {return wlFile_;}				///< Get worldline file
		fstream & permCycleFile() {return permCycleFile_;}	///< Get permutation cycle file
		fstream & obdmFile() {return obdmFile_;}			///< Get one body density matrix file
		fstream & pairFile() {return pairFile_;}			///< Get pair correlation file
		fstream & radialFile() {return radialFile_;}		///< Get radial density file
		fstream & wormFile() {return wormFile_;}			///< Get worm properties file
		fstream & numberFile() {return numberFile_;}		///< Get number distribution file
        fstream & positionFile() {return positionFile_;}    ///< Get particle position file
        fstream & windDensFile() {return windDensFile_;}    ///< Get Winding number density

		fstream & debugFile();

		/* The cylinder estimator files */
		fstream & cylEstimatorFile() {return cylEstimatorFile_;} ///< Get the cylinder estimator file
		fstream & cylSuperFile() {return cylSuperFile_;}		 ///< Get the cylinder superfluid file
		fstream & cylNumberFile() {return cylNumberFile_;}		 ///< Get the cylinder number file 
		fstream & cylObdmFile() {return cylObdmFile_;}			 ///< Get the cylinder OBDM file
		fstream & cylPairFile() {return cylPairFile_;}			 ///< Get the cylinder pair CF file
		fstream & cylPotentialFile() { return cylPotentialFile_;}///< Get the cylinder potential file

		/* Reset the state file */
		void resetStateFile(ios_base::openmode);	

		/* Reset the fixed file */
		void resetFixedFile();

        /* Reset the Position Histogram file */
        void resetPositionFile(ios_base::openmode);

        /* Reset the Winding Number Density file */
        void resetWindDensFile(ios_base::openmode);

	protected:
		Communicator();									
		~Communicator();								
		Communicator(const Communicator&);				///< Copy constructor
		Communicator& operator= (const Communicator&);	///< Singleton equals

	private:
		fstream logFile_;			// The logging simulation parameters file
		fstream estimatorFile_;		// Single value estimators
		fstream superFile_;			// Superfluid estimators
		fstream debugFile_;			// Debugging output
		fstream stateFile_;			// Diagonal path information
		fstream initFile_;			// Initial configuration state
		fstream fixedFile_;			// Location of fixed non-updatable particles
		fstream wlFile_;			// Particle worldlines
		fstream permCycleFile_;		// Permutation cycle statistics
		fstream obdmFile_;			// One body density matrix
		fstream pairFile_;			// Pair correlation function
		fstream radialFile_;		// Radial density
		fstream wormFile_;			// Worm properties
		fstream numberFile_;		// The number distribution
        fstream positionFile_;      // Particle Positions
        fstream windDensFile_;      // Winding Number Density

		/* All the cylinder estimator files */
		fstream cylEstimatorFile_;
		fstream cylSuperFile_;			
		fstream cylNumberFile_;		
		fstream cylObdmFile_;			
		fstream cylPairFile_;			
		fstream cylPotentialFile_;			

		string ensemble;			// The type of ensemble
		string dataName;			// The labelling scheme of the output files

		string stateName;			// The name of the state file
		string fixedName;			// The name of the fixed file
        string positionName;        // The name of the position density file
        string windDensName;        // The name of the winding # density file

		map <string,fstream*> file;	    // The file map
		map <string,fstream*> cylFile;	// The cylinder file map

        /* Opens a file */
		void openFile(const string , fstream *, ios_base::openmode);

        /* Makes sure we have a unique PIMCID */
        void getUniqueID(const double);
};

/**************************************************************************//**
 *  Global public access to the communcator singleton.
******************************************************************************/
inline CCommunicator* ccommunicate() {
	CCommunicator *temp = CCommunicator::getInstance();
	return temp;
}


/**************************************************************************//**
 *  Global public access to the communcator singleton.
******************************************************************************/
inline Communicator* communicate() {
	Communicator *temp = Communicator::getInstance();
	return temp;
}
#endif

