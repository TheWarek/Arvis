#ifndef __3DVisualServerNewNodeExecutor_H__
#define __3DVisualServerNewNodeExecutor_H__

#include "Network/executors/AbstractExecutor.h"

namespace Network {

    class ServerNewNodeExecutor : public AbstractExecutor {

    private:
        QDataStream *stream;

    public:
        void execute();
        void setDataStream(QDataStream *ds) {stream=ds;}
        static const quint8 INSTRUCTION_NUMBER = 20;
    };

}

#endif