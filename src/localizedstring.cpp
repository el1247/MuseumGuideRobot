#include "localizedstring.h"

LocalizedString::LocalizedString(const char * str){
    m_originalString(str), m_localizedString(tr(str))
}
