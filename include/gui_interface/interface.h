#ifndef INTERFACE_H
#define INTERFACE_H

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <vector>

#include "view.h"

class AveragingBuffer {
public:
    int buffer_size;
    int i = 0;
    int total_sum = 0;

    std::vector<int> buffer;
    AveragingBuffer(int initBufferSize): buffer_size(initBufferSize) {
        for (int k = 0; k < buffer_size; k++)
        {
            buffer.push_back(0);
        }
    };

    void enqueue(int x) {
        i++;
        i = i % buffer_size;
        total_sum -= buffer[i];
        total_sum += x;
        buffer[i] = x;
    }

    int average() {
        return total_sum / buffer_size;
    }
};

class GUI_Interface {
public:
    Renderer *renderer;
    ImGuiIO *io;

    float f = 0.0;

    // Total chunk initialisations
    int chunkInitialisations = 0;

    // Total chunk deletions
    int chunkDeletions = 0 ;

    // Chunk mesh gen time
    AveragingBuffer *chunkMeshGenTime = new AveragingBuffer(125);

    // Chunk mesh initialisation time
    AveragingBuffer *chunkInitTime = new AveragingBuffer(125);

    // Time to delete chunks
    AveragingBuffer *chunkDelTime = new AveragingBuffer(125);

    GUI_Interface(Renderer *initRenderer);

    void update();
};

#endif