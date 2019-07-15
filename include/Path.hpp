#ifndef PATH_HPP
#define PATH_HPP

class Path {
 public:
  /**
   * user.csv path
   */
  static const char *userPath;

  /**
   * meeting.csv path
   */
  static const char *meetingPath;
  
  
  static const char *logPath;
};

const char *Path::meetingPath = "../data/meetings.csv";
const char *Path::userPath = "../data/users.csv";
const char *Path::logPath = "../data/log.txt";
#endif
