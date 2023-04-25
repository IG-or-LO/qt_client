#include <emodjis.h>

namespace emojicpp {

QString getemoji(QString s) {
    int index = -1;
    int sLen = s.size();
    for (int i = 0; i < sLen; i++) {
        if (s[i] == *L":") {
            if (index == -1) {
                index = i;
            }
            else {
                if (i - index==1) {
                    index = i;
                    continue;
                }
                QMap<QString, QString>::iterator it;

                //std::map<std::string, std::string>::iterator it;
                it = EMOJIS.find(s.mid(index, i - index + 1));
                if (it == EMOJIS.end()) {
                    index = i;
                    continue;
                }
                QString emo = it.value();
                // replace from index to i
                //std::cout << s.substr(index, i - index + 1) << std::endl; // <---- uncomment to see what text is replaced, might be good for debugging
                s.replace(index, i - index + 1 , emo);
                int goBack = i - index + 1 - emo.size();
                sLen -= goBack;
                i -= goBack;
                index = -1;
            }
        }
    }
    return s;
}
}
