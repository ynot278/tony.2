#define N_LICENSE 20
#define TIMER 100

int getlicense(void); //Blocks until a license is available.

int returnlicense(void); //Increments the number of available licenses.

int initlicense(void); //Performs any needed initialization of the license object.

int addtolicenses(int n); //Adds n licenses to the number available.

int removelicenses(int n); //Decrements the number of licenses by n.

void logmsg(const char * msg); //Write the specified message to the log file.  There is only one log file.  This function will treat the log file as a critical resource.  It will open the file to append the message and close the file after appending the message.