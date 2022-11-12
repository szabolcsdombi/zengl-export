#include <Python.h>

const int MAX_ATTACHMENTS = 16;
const int MAX_UNIFORM_BUFFER_BINDINGS = 16;
const int MAX_SAMPLER_BINDINGS = 64;
const int MAX_UNIFORM_BINDINGS = 64;

struct VertexFormat {
    int type;
    int size;
    int normalize;
    int integer;
};

struct ImageFormat {
    int internal_format;
    int format;
    int type;
    int components;
    int pixel_size;
    int buffer;
    int color;
    int clear_type;
};

struct UniformBufferBinding {
    int buffer;
    int offset;
    int size;
};

struct SamplerBinding {
    int sampler;
    int target;
    int image;
};

struct UniformBinding {
    int values;
    int location;
    int count;
    int type;
    union {
        int int_values[1];
        unsigned uint_values[1];
        float float_values[1];
    };
};

struct StencilSettings {
    int fail_op;
    int pass_op;
    int depth_fail_op;
    int compare_op;
    int compare_mask;
    int write_mask;
    int reference;
};

union Viewport {
    unsigned long long viewport;
    struct {
        short x;
        short y;
        short width;
        short height;
    };
};

union ClearValue {
    float clear_floats[4];
    int clear_ints[4];
    unsigned int clear_uints[4];
};

struct IntPair {
    int x;
    int y;
};

struct ModuleState {
    PyObject * helper;
    PyObject * empty_tuple;
    PyObject * str_none;
    PyObject * float_one;
    PyObject * default_color_mask;
    PyTypeObject * Context_type;
    PyTypeObject * Buffer_type;
    PyTypeObject * Image_type;
    PyTypeObject * Pipeline_type;
    PyTypeObject * ImageFace_type;
    PyTypeObject * DescriptorSetBuffers_type;
    PyTypeObject * DescriptorSetImages_type;
    PyTypeObject * GlobalSettings_type;
    PyTypeObject * GLObject_type;
};

struct GCHeader {
    PyObject_HEAD
    GCHeader * gc_prev;
    GCHeader * gc_next;
};

struct GLObject {
    PyObject_HEAD
    int uses;
    int obj;
};

struct DescriptorSetBuffers {
    PyObject_HEAD
    int uses;
    int buffers;
    UniformBufferBinding binding[MAX_UNIFORM_BUFFER_BINDINGS];
};

struct DescriptorSetImages {
    PyObject_HEAD
    int uses;
    int samplers;
    SamplerBinding binding[MAX_SAMPLER_BINDINGS];
    GLObject * sampler[MAX_SAMPLER_BINDINGS];
};

struct GlobalSettings {
    PyObject_HEAD
    int uses;
    unsigned long long color_mask;
    int primitive_restart;
    int cull_face;
    int depth_test;
    int depth_write;
    int depth_func;
    int stencil_test;
    StencilSettings stencil_front;
    StencilSettings stencil_back;
    int blend_enable;
    int blend_op_color;
    int blend_op_alpha;
    int blend_src_color;
    int blend_dst_color;
    int blend_src_alpha;
    int blend_dst_alpha;
    int polygon_offset;
    float polygon_offset_factor;
    float polygon_offset_units;
    int attachments;
    int is_mask_default;
    int is_stencil_default;
    int is_blend_default;
};

struct Context {
    PyObject_HEAD
    GCHeader * gc_prev;
    GCHeader * gc_next;
    ModuleState * module_state;
    PyObject * descriptor_set_buffers_cache;
    PyObject * descriptor_set_images_cache;
    PyObject * global_settings_cache;
    PyObject * sampler_cache;
    PyObject * vertex_array_cache;
    PyObject * framebuffer_cache;
    PyObject * program_cache;
    PyObject * shader_cache;
    PyObject * includes;
    PyObject * limits;
    PyObject * info;
    DescriptorSetBuffers * current_buffers;
    DescriptorSetImages * current_images;
    GlobalSettings * current_global_settings;
    Viewport viewport;
    int is_mask_default;
    int is_stencil_default;
    int is_blend_default;
    int current_attachments;
    int current_framebuffer;
    int current_program;
    int current_vertex_array;
    int current_clear_mask;
    int default_texture_unit;
    int max_samples;
    int mapped_buffers;
    int screen;
    // GLMethods gl;
};

struct Buffer {
    PyObject_HEAD
    GCHeader * gc_prev;
    GCHeader * gc_next;
    Context * ctx;
    int buffer;
    int size;
    int dynamic;
    int mapped;
};

struct Image {
    PyObject_HEAD
    GCHeader * gc_prev;
    GCHeader * gc_next;
    Context * ctx;
    PyObject * size;
    GLObject * framebuffer;
    PyObject * faces;
    ClearValue clear_value;
    ImageFormat format;
    int image;
    int width;
    int height;
    int samples;
    int array;
    int cubemap;
    int target;
    int renderbuffer;
    int max_level;
};

struct Pipeline {
    PyObject_HEAD
    GCHeader * gc_prev;
    GCHeader * gc_next;
    Context * ctx;
    DescriptorSetBuffers * descriptor_set_buffers;
    DescriptorSetImages * descriptor_set_images;
    GlobalSettings * global_settings;
    GLObject * framebuffer;
    GLObject * vertex_array;
    GLObject * program;
    PyObject * uniform_map;
    char * uniform_data;
    int uniform_count;
    int topology;
    int vertex_count;
    int instance_count;
    int first_vertex;
    int index_type;
    int index_size;
    Viewport viewport;
};

struct ImageFace {
    PyObject_HEAD
    GCHeader * gc_prev;
    GCHeader * gc_next;
    Context * ctx;
    Image * image;
    GLObject * framebuffer;
    PyObject * size;
    int width;
    int height;
    int layer;
    int level;
    int samples;
    int color;
};

PyObject * json;
PyObject * regex;

VertexFormat get_vertex_format(const char * format) {
    if (!strcmp(format, "uint8x2")) return {0x1401, 2, false, true};
    if (!strcmp(format, "uint8x4")) return {0x1401, 4, false, true};
    if (!strcmp(format, "sint8x2")) return {0x1400, 2, false, true};
    if (!strcmp(format, "sint8x4")) return {0x1400, 4, false, true};
    if (!strcmp(format, "unorm8x2")) return {0x1401, 2, true, false};
    if (!strcmp(format, "unorm8x4")) return {0x1401, 4, true, false};
    if (!strcmp(format, "snorm8x2")) return {0x1400, 2, true, false};
    if (!strcmp(format, "snorm8x4")) return {0x1400, 4, true, false};
    if (!strcmp(format, "uint16x2")) return {0x1403, 2, false, true};
    if (!strcmp(format, "uint16x4")) return {0x1403, 4, false, true};
    if (!strcmp(format, "sint16x2")) return {0x1402, 2, false, true};
    if (!strcmp(format, "sint16x4")) return {0x1402, 4, false, true};
    if (!strcmp(format, "unorm16x2")) return {0x1403, 2, true, false};
    if (!strcmp(format, "unorm16x4")) return {0x1403, 4, true, false};
    if (!strcmp(format, "snorm16x2")) return {0x1402, 2, true, false};
    if (!strcmp(format, "snorm16x4")) return {0x1402, 4, true, false};
    if (!strcmp(format, "float16x2")) return {0x140b, 2, false, false};
    if (!strcmp(format, "float16x4")) return {0x140b, 4, false, false};
    if (!strcmp(format, "float32")) return {0x1406, 1, false, false};
    if (!strcmp(format, "float32x2")) return {0x1406, 2, false, false};
    if (!strcmp(format, "float32x3")) return {0x1406, 3, false, false};
    if (!strcmp(format, "float32x4")) return {0x1406, 4, false, false};
    if (!strcmp(format, "uint32")) return {0x1405, 1, false, true};
    if (!strcmp(format, "uint32x2")) return {0x1405, 2, false, true};
    if (!strcmp(format, "uint32x3")) return {0x1405, 3, false, true};
    if (!strcmp(format, "uint32x4")) return {0x1405, 4, false, true};
    if (!strcmp(format, "sint32")) return {0x1404, 1, false, true};
    if (!strcmp(format, "sint32x2")) return {0x1404, 2, false, true};
    if (!strcmp(format, "sint32x3")) return {0x1404, 3, false, true};
    if (!strcmp(format, "sint32x4")) return {0x1404, 4, false, true};
    return {};
}

const char * str_shader_type(int arg) {
    switch (arg) {
        case 0x8b31: return "GL_VERTEX_SHADER";
        case 0x8b30: return "GL_FRAGMENT_SHADER";
    }
    return "";
}

const char * str_texture_target(int arg) {
    switch (arg) {
        case 0x0de1: return "GL_TEXTURE_2D";
        case 0x8513: return "GL_TEXTURE_CUBE_MAP";
        case 0x8c1a: return "GL_TEXTURE_2D_ARRAY";
    }
    return "";
}

const char * str_format(int arg) {
    switch (arg) {
        case 0x1400: return "GL_BYTE";
        case 0x1401: return "GL_UNSIGNED_BYTE";
        case 0x1402: return "GL_SHORT";
        case 0x1403: return "GL_UNSIGNED_SHORT";
        case 0x1404: return "GL_INT";
        case 0x1405: return "GL_UNSIGNED_INT";
        case 0x1406: return "GL_FLOAT";
        case 0x140b: return "GL_HALF_FLOAT";
        case 0x84fa: return "GL_UNSIGNED_INT_24_8";
    }
    return "";
}

const char * str_pixel_format(int arg) {
    switch (arg) {
        case 0x1903: return "GL_RED";
        case 0x8d94: return "GL_RED_INTEGER";
        case 0x8227: return "GL_RG";
        case 0x8228: return "GL_RG_INTEGER";
        case 0x1908: return "GL_RGBA";
        case 0x8d99: return "GL_RGBA_INTEGER";
        case 0x80e1: return "GL_BGRA";
        case 0x1902: return "GL_DEPTH_COMPONENT";
        case 0x84f9: return "GL_DEPTH_STENCIL";
        case 0x1901: return "GL_STENCIL_INDEX";
    }
    return "";
}

const char * str_internal_format(int arg) {
    switch (arg) {
        case 0x8229: return "GL_R8";
        case 0x822b: return "GL_RG8";
        case 0x8058: return "GL_RGBA8";
        case 0x8f94: return "GL_R8_SNORM";
        case 0x8f95: return "GL_RG8_SNORM";
        case 0x8f97: return "GL_RGBA8_SNORM";
        case 0x8232: return "GL_R8UI";
        case 0x8238: return "GL_RG8UI";
        case 0x8d7c: return "GL_RGBA8UI";
        case 0x8234: return "GL_R16UI";
        case 0x823a: return "GL_RG16UI";
        case 0x8d76: return "GL_RGBA16UI";
        case 0x8236: return "GL_R32UI";
        case 0x823c: return "GL_RG32UI";
        case 0x8d70: return "GL_RGBA32UI";
        case 0x8231: return "GL_R8I";
        case 0x8237: return "GL_RG8I";
        case 0x8d8e: return "GL_RGBA8I";
        case 0x8233: return "GL_R16I";
        case 0x8239: return "GL_RG16I";
        case 0x8d88: return "GL_RGBA16I";
        case 0x8235: return "GL_R32I";
        case 0x823b: return "GL_RG32I";
        case 0x8d82: return "GL_RGBA32I";
        case 0x822d: return "GL_R16F";
        case 0x822f: return "GL_RG16F";
        case 0x881a: return "GL_RGBA16F";
        case 0x822e: return "GL_R32F";
        case 0x8230: return "GL_RG32F";
        case 0x8814: return "GL_RGBA32F";
        case 0x8c43: return "GL_SRGB8_ALPHA8";
        case 0x8d48: return "GL_STENCIL_INDEX8";
        case 0x81a5: return "GL_DEPTH_COMPONENT16";
        case 0x81a6: return "GL_DEPTH_COMPONENT24";
        case 0x88f0: return "GL_DEPTH24_STENCIL8";
        case 0x8cac: return "GL_DEPTH_COMPONENT32F";
    }
    return "";
}

const char * str_topology(int arg) {
    switch (arg) {
        case 0: return "GL_POINTS";
        case 1: return "GL_LINES";
        case 2: return "GL_LINE_LOOP";
        case 3: return "GL_LINE_STRIP";
        case 4: return "GL_TRIANGLES";
        case 5: return "GL_TRIANGLE_STRIP";
        case 6: return "GL_TRIANGLE_FAN";
    }
    return "";
}

const char * str_cubemap_face(int arg) {
    switch (arg) {
        case 0: return "GL_TEXTURE_CUBE_MAP_POSITIVE_X";
        case 1: return "GL_TEXTURE_CUBE_MAP_NEGATIVE_X";
        case 2: return "GL_TEXTURE_CUBE_MAP_POSITIVE_Y";
        case 3: return "GL_TEXTURE_CUBE_MAP_NEGATIVE_Y";
        case 4: return "GL_TEXTURE_CUBE_MAP_POSITIVE_Z";
        case 5: return "GL_TEXTURE_CUBE_MAP_NEGATIVE_Z";
    }
    return "";
}

const char * str_cull_face(int arg) {
    switch (arg) {
        case 0x0404: return "GL_FRONT";
        case 0x0405: return "GL_BACK";
        case 0x0408: return "GL_FRONT_AND_BACK";
        case 0x0000: return "GL_NONE";
    }
    return "";
}

const char * str_filter(int arg) {
    switch (arg) {
        case 0x2600: return "GL_NEAREST";
        case 0x2601: return "GL_LINEAR";
        case 0x2700: return "GL_NEAREST_MIPMAP_NEAREST";
        case 0x2701: return "GL_LINEAR_MIPMAP_NEAREST";
        case 0x2702: return "GL_NEAREST_MIPMAP_LINEAR";
        case 0x2703: return "GL_LINEAR_MIPMAP_LINEAR";
    }
    return "";
}

const char * str_texture_wrap(int arg) {
    switch (arg) {
        case 0x2901: return "GL_REPEAT";
        case 0x812f: return "GL_CLAMP_TO_EDGE";
        case 0x8370: return "GL_MIRRORED_REPEAT";
    }
    return "";
}

const char * str_compare_mode(int arg) {
    switch (arg) {
        case 0x884e: return "GL_REF_TO_TEXTURE";
        case 0x0000: return "GL_NONE";
    }
    return "";
}

const char * str_compare_func(int arg) {
    switch (arg) {
        case 0x0200: return "GL_NEVER";
        case 0x0201: return "GL_LESS";
        case 0x0202: return "GL_EQUAL";
        case 0x0203: return "GL_LEQUAL";
        case 0x0204: return "GL_GREATER";
        case 0x0205: return "GL_NOTEQUAL";
        case 0x0206: return "GL_GEQUAL";
        case 0x0207: return "GL_ALWAYS";
    }
    return "";
}

const char * str_blend_func(int arg) {
    switch (arg) {
        case 0x8006: return "GL_ADD";
        case 0x800a: return "GL_SUBTRACT";
        case 0x800b: return "GL_REVERSE_SUBTRACT";
        case 0x8007: return "GL_MIN";
        case 0x8008: return "GL_MAX";
    }
    return "";
}

const char * str_blend_constant(int arg) {
    switch (arg) {
        case 0x0000: return "GL_ZERO";
        case 0x0001: return "GL_ONE";
        case 0x0300: return "GL_SRC_COLOR";
        case 0x0301: return "GL_ONE_MINUS_SRC_COLOR";
        case 0x0302: return "GL_SRC_ALPHA";
        case 0x0303: return "GL_ONE_MINUS_SRC_ALPHA";
        case 0x0304: return "GL_DST_ALPHA";
        case 0x0305: return "GL_ONE_MINUS_DST_ALPHA";
        case 0x0306: return "GL_DST_COLOR";
        case 0x0307: return "GL_ONE_MINUS_DST_COLOR";
        case 0x0308: return "GL_SRC_ALPHA_SATURATE";
        case 0x8001: return "GL_CONSTANT_COLOR";
        case 0x8002: return "GL_ONE_MINUS_CONSTANT_COLOR";
        case 0x8003: return "GL_CONSTANT_ALPHA";
        case 0x8004: return "GL_ONE_MINUS_CONSTANT_ALPHA";
        case 0x8589: return "GL_SRC1_ALPHA";
        case 0x88f9: return "GL_SRC1_COLOR";
        case 0x88fa: return "GL_ONE_MINUS_SRC1_COLOR";
        case 0x88fb: return "GL_ONE_MINUS_SRC1_ALPHA";
    }
    return "";
}

const char * str_stencil_op(int arg) {
    switch (arg) {
        case 0x0000: return "GL_ZERO";
        case 0x1e00: return "GL_KEEP";
        case 0x1e01: return "GL_REPLACE";
        case 0x1e02: return "GL_INCR";
        case 0x1e03: return "GL_DECR";
        case 0x150a: return "GL_INVERT";
        case 0x8507: return "GL_INCR_WRAP";
        case 0x8508: return "GL_DECR_WRAP";
    }
    return "";
}

void print_buffer(char *& s, Buffer * buffer) {
    s += sprintf(s, "unsigned buffer%d = 0;\n", buffer->buffer);
    s += sprintf(s, "glGenBuffers(1, &buffer%d);\n", buffer->buffer);
    s += sprintf(s, "glBindBuffer(GL_ARRAY_BUFFER, buffer%d);\n", buffer->buffer);
    s += sprintf(s, "glBufferData(GL_ARRAY_BUFFER, %d, data, %s);\n", buffer->size, buffer->dynamic ? "GL_DYNAMIC_DRAW" : "GL_STATIC_DRAW");
}

void print_image(char *& s, Image * image) {
    if (image->renderbuffer) {
        s += sprintf(s, "unsigned renderbuffer%d = 0;\n", image->image);
        s += sprintf(s, "glGenRenderbuffers(1, &renderbuffer%d);\n", image->image);
        s += sprintf(s, "glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer%d);\n", image->image);
        s += sprintf(s, "glRenderbufferStorageMultisample(GL_RENDERBUFFER, %d, %s, %d, %d);\n", image->samples > 1 ? image->samples : 0, str_internal_format(image->format.internal_format), image->width, image->height);
    } else {
        s += sprintf(s, "unsigned image%d = 0;\n", image->image);
        s += sprintf(s, "glGenTextures(1, &image%d);\n", image->image);
        s += sprintf(s, "glBindTexture(%s, image%d);\n", str_texture_target(image->target), image->image);
        if (image->cubemap) {
            for (int i = 0; i < 6; ++i) {
                s += sprintf(s, "glTexImage2D(%s, 0, %s, %d, %d, 0, %s, %s, data);\n", str_cubemap_face(i), str_internal_format(image->format.internal_format), image->width, image->height, str_pixel_format(image->format.format), str_format(image->format.type));
            }
        } else if (image->array) {
            s += sprintf(s, "glTexImage3D(%s, 0, %s, %d, %d, %d, 0, %s, %s, data);\n", str_texture_target(image->target), str_internal_format(image->format.internal_format), image->width, image->height, image->array, str_pixel_format(image->format.format), str_format(image->format.type));
        } else {
            s += sprintf(s, "glTexImage2D(%s, 0, %s, %d, %d, 0, %s, %s, data);\n", str_texture_target(image->target), str_internal_format(image->format.internal_format), image->width, image->height, str_pixel_format(image->format.format), str_format(image->format.type));
        }
    }
}

void print_framebuffer_attachment(char *& s, ImageFace * face, int idx) {
    char color_attachment[32];
    const char * attachment = face->image->format.buffer == 0x1801 ? "GL_DEPTH_ATTACHMENT" : face->image->format.buffer == 0x1802 ? "GL_STENCIL_ATTACHMENT" : "GL_DEPTH_STENCIL_ATTACHMENT";
    if (idx >= 0) {
        sprintf(color_attachment, "GL_COLOR_ATTACHMENT%d", idx);
        attachment = color_attachment;
    }
    if (face->image->renderbuffer) {
        s += sprintf(s, "glFramebufferRenderbuffer(GL_FRAMEBUFFER, %s, GL_RENDERBUFFER, renderbuffer%d);\n", attachment, face->image->image);
    } else if (face->image->cubemap) {
        s += sprintf(s, "glFramebufferTexture2D(GL_FRAMEBUFFER, %s, %s, image%d, %d);\n", attachment, str_cubemap_face(face->layer), face->image->image, face->level);
    } else if (face->image->array) {
        s += sprintf(s, "glFramebufferTextureLayer(GL_FRAMEBUFFER, %s, image%d, %d, %d);\n", attachment, face->image->image, face->level, face->layer);
    } else {
        s += sprintf(s, "glFramebufferTexture2D(GL_FRAMEBUFFER, %s, GL_TEXTURE_2D, image%d, %d);\n", attachment, face->image->image, face->level);
    }
}

void print_framebuffer(char *& s, int framebuffer, PyObject * attachments) {
    PyObject * color_attachments = PyTuple_GetItem(attachments, 1);
    PyObject * depth_stencil_attachment = PyTuple_GetItem(attachments, 2);
    int color_attachment_count = (int)PyTuple_Size(color_attachments);

    s += sprintf(s, "unsigned framebuffer%d = 0;\n", framebuffer);
    s += sprintf(s, "glGenFramebuffers(1, &framebuffer%d);\n", framebuffer);
    s += sprintf(s, "glBindFramebuffer(GL_FRAMEBUFFER, framebuffer%d);\n", framebuffer);

    for (int i = 0; i < color_attachment_count; ++i) {
        ImageFace * face = (ImageFace *)PyTuple_GetItem(color_attachments, i);
        print_framebuffer_attachment(s, face, i);
    }

    if (depth_stencil_attachment != Py_None) {
        ImageFace * face = (ImageFace *)depth_stencil_attachment;
        print_framebuffer_attachment(s, face, -1);
    }

    s += sprintf(s, "unsigned draw_buffers%d[] = {", framebuffer);
    for (int i = 0; i < color_attachment_count; ++i) {
        s += sprintf(s, "%sGL_COLOR_ATTACHMENT%d", i ? ", " : "", i);
    }
    s += sprintf(s, "};\n");

    s += sprintf(s, "glDrawBuffers(%d, draw_buffers%d);\n", color_attachment_count, framebuffer);
    s += sprintf(s, "glReadBuffer(%s);\n", color_attachment_count ? "GL_COLOR_ATTACHMENT0" : "GL_NONE");
}

void print_shader(char *& s, PyObject * src, int shader, int type) {
    s += sprintf(s, "const char * src%d = %s;\n", shader, PyUnicode_AsUTF8(src));
    s += sprintf(s, "unsigned shader%d = glCreateShader(%s);\n", shader, str_shader_type(type));
    s += sprintf(s, "glShaderSource(shader%d, 1, &src%d, NULL);\n", shader, shader);
    s += sprintf(s, "glCompileShader(shader%d);\n", shader);
}

void print_program(char *& s, int program, int vertex_shader, int fragment_shader) {
    s += sprintf(s, "unsigned program%d = glCreateProgram();\n", program);
    s += sprintf(s, "glAttachShader(program%d, shader%d);\n", program, vertex_shader);
    s += sprintf(s, "glAttachShader(program%d, shader%d);\n", program, fragment_shader);
    s += sprintf(s, "glLinkProgram(program%d);\n", program);
}

void print_vertex_array(char *& s, int vertex_array, PyObject * bindings) {
    int length = (int)PyTuple_Size(bindings);
    PyObject ** seq = PySequence_Fast_ITEMS(bindings);
    PyObject * index_buffer = seq[0];

    s += sprintf(s, "unsigned vertex_array%d = 0;\n", vertex_array);
    s += sprintf(s, "glGenVertexArrays(1, &vertex_array%d);\n", vertex_array);
    s += sprintf(s, "glBindVertexArray(vertex_array%d);\n", vertex_array);

    for (int i = 1; i < length; i += 6) {
        Buffer * buffer = (Buffer *)seq[i + 0];
        int location = PyLong_AsLong(seq[i + 1]);
        int offset = PyLong_AsLong(seq[i + 2]);
        int stride = PyLong_AsLong(seq[i + 3]);
        int divisor = PyLong_AsLong(seq[i + 4]);
        VertexFormat format = get_vertex_format(PyUnicode_AsUTF8(seq[i + 5]));
        s += sprintf(s, "glBindBuffer(GL_ARRAY_BUFFER, buffer%d);\n", buffer->buffer);
        if (format.integer) {
            s += sprintf(s, "glVertexAttribIPointer(%d, %d, %s, %d, %d);\n", location, format.size, str_format(format.type), stride, offset);
        } else {
            s += sprintf(s, "glVertexAttribPointer(%d, %d, %s, %s, %d, %d);\n", location, format.size, str_format(format.type), format.normalize ? "true" : "false", stride, offset);
        }
        s += sprintf(s, "glVertexAttribDivisor(%d, %d);\n", location, divisor);
        s += sprintf(s, "glEnableVertexAttribArray(%d);\n", location);
    }

    if (index_buffer != Py_None) {
        Buffer * buffer = (Buffer *)index_buffer;
        s += sprintf(s, "glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer%d);\n", buffer->buffer);
    }
}

void print_sampler(char *& s, int sampler, PyObject * params) {
    PyObject ** seq = PySequence_Fast_ITEMS(params);

    s += sprintf(s, "unsigned sampler%d = 0;\n", sampler);
    s += sprintf(s, "glGenSamplers(1, &sampler%d);\n", sampler);
    s += sprintf(s, "glSamplerParameteri(sampler%d, GL_TEXTURE_MIN_FILTER, %s);\n", sampler, str_filter(PyLong_AsLong(seq[0])));
    s += sprintf(s, "glSamplerParameteri(sampler%d, GL_TEXTURE_MAG_FILTER, %s);\n", sampler, str_filter(PyLong_AsLong(seq[1])));
    s += sprintf(s, "glSamplerParameterf(sampler%d, GL_TEXTURE_MIN_LOD, %f);\n", sampler, PyFloat_AsDouble(seq[2]));
    s += sprintf(s, "glSamplerParameterf(sampler%d, GL_TEXTURE_MAX_LOD, %f);\n", sampler, PyFloat_AsDouble(seq[3]));
    s += sprintf(s, "glSamplerParameterf(sampler%d, GL_TEXTURE_LOD_BIAS, %f);\n", sampler, PyFloat_AsDouble(seq[4]));
    s += sprintf(s, "glSamplerParameteri(sampler%d, GL_TEXTURE_WRAP_S, %s);\n", sampler, str_texture_wrap(PyLong_AsLong(seq[5])));
    s += sprintf(s, "glSamplerParameteri(sampler%d, GL_TEXTURE_WRAP_T, %s);\n", sampler, str_texture_wrap(PyLong_AsLong(seq[6])));
    s += sprintf(s, "glSamplerParameteri(sampler%d, GL_TEXTURE_WRAP_R, %s);\n", sampler, str_texture_wrap(PyLong_AsLong(seq[7])));
    s += sprintf(s, "glSamplerParameteri(sampler%d, GL_TEXTURE_COMPARE_MODE, %s);\n", sampler, str_compare_mode(PyLong_AsLong(seq[8])));
    s += sprintf(s, "glSamplerParameteri(sampler%d, GL_TEXTURE_COMPARE_FUNC, %s);\n", sampler, str_compare_func(PyLong_AsLong(seq[9])));
    s += sprintf(s, "glSamplerParameterf(sampler%d, GL_TEXTURE_MAX_ANISOTROPY, %f);\n", sampler, PyFloat_AsDouble(seq[10]));

    float r = (float)PyFloat_AsDouble(seq[11]);
    float g = (float)PyFloat_AsDouble(seq[12]);
    float b = (float)PyFloat_AsDouble(seq[13]);
    float a = (float)PyFloat_AsDouble(seq[14]);

    s += sprintf(s, "float border%d[] = {%f, %f, %f, %f};\n", sampler, r, g, b, a);
    s += sprintf(s, "glSamplerParameterfv(sampler%d, GL_TEXTURE_BORDER_COLOR, border%d);\n", sampler, sampler);
}

void print_settings(char *& s, GlobalSettings * settings) {
    s += sprintf(s, "%s(GL_PRIMITIVE_RESTART);\n", settings->primitive_restart ? "glEnable" : "glDisable");
    s += sprintf(s, "%s(GL_POLYGON_OFFSET_FILL);\n", settings->polygon_offset ? "glEnable" : "glDisable");
    s += sprintf(s, "%s(GL_CULL_FACE);\n", settings->cull_face ? "glEnable" : "glDisable");
    s += sprintf(s, "%s(GL_DEPTH_TEST);\n", settings->depth_test ? "glEnable" : "glDisable");
    s += sprintf(s, "%s(GL_STENCIL_TEST);\n", settings->stencil_test ? "glEnable" : "glDisable");
    if (settings->polygon_offset) {
        s += sprintf(s, "glPolygonOffset(%f, %f);\n", settings->polygon_offset_factor, settings->polygon_offset_units);
    }
    if (settings->cull_face) {
        s += sprintf(s, "glCullFace(%s);\n", str_cull_face(settings->cull_face));
    }
    if (settings->depth_test) {
        s += sprintf(s, "glDepthFunc(%s);\n", str_compare_func(settings->depth_func));
    }
    s += sprintf(s, "glStencilMaskSeparate(GL_FRONT, 0x%02x);\n", settings->stencil_front.write_mask);
    s += sprintf(s, "glStencilMaskSeparate(GL_BACK, 0x%02x);\n", settings->stencil_back.write_mask);
    s += sprintf(s, "glStencilFuncSeparate(GL_FRONT, %s, 0x%02x, 0x%02x);\n", str_compare_func(settings->stencil_front.compare_op), settings->stencil_front.reference, settings->stencil_front.compare_mask);
    s += sprintf(s, "glStencilFuncSeparate(GL_BACK, %s, 0x%02x, 0x%02x);\n", str_compare_func(settings->stencil_back.compare_op), settings->stencil_back.reference, settings->stencil_back.compare_mask);
    s += sprintf(s, "glStencilOpSeparate(GL_FRONT, %s, %s, %s);\n", str_stencil_op(settings->stencil_front.fail_op), str_stencil_op(settings->stencil_front.pass_op), str_stencil_op(settings->stencil_front.depth_fail_op));
    s += sprintf(s, "glStencilOpSeparate(GL_BACK, %s, %s, %s);\n", str_stencil_op(settings->stencil_back.fail_op), str_stencil_op(settings->stencil_back.pass_op), str_stencil_op(settings->stencil_back.depth_fail_op));
    s += sprintf(s, "glDepthMask(%s);\n", settings->depth_write ? "true" : "false");
    for (int i = 0; i < settings->attachments; ++i) {
        bool r = settings->color_mask >> (i * 4 + 0) & 1;
        bool g = settings->color_mask >> (i * 4 + 1) & 1;
        bool b = settings->color_mask >> (i * 4 + 2) & 1;
        bool a = settings->color_mask >> (i * 4 + 3) & 1;
        s += sprintf(s, "glColorMaski(%d, %s, %s, %s, %s);\n", i, r ? "true" : "false", g ? "true" : "false", b ? "true" : "false", a ? "true" : "false");
    }
    s += sprintf(s, "glBlendEquationSeparate(%s, %s);\n", str_blend_func(settings->blend_op_color), str_blend_func(settings->blend_op_alpha));
    s += sprintf(s, "glBlendFuncSeparate(%s, %s, %s, %s);\n", str_blend_constant(settings->blend_src_color), str_blend_constant(settings->blend_dst_color), str_blend_constant(settings->blend_src_alpha), str_blend_constant(settings->blend_dst_alpha));
    for (int i = 0; i < settings->attachments; ++i) {
        s += sprintf(s, "%s(GL_BLEND, %d);\n", (settings->blend_enable >> i & 1) ? "glEnablei" : "glDisablei", i);
    }
}

void print_pipeline(char *& s, Pipeline * self) {
    print_settings(s, self->global_settings);
    s += sprintf(s, "glViewport(%d, %d, %d, %d);\n", self->viewport.x, self->viewport.y, self->viewport.width, self->viewport.height);
    s += sprintf(s, "glBindFramebuffer(GL_FRAMEBUFFER, framebuffer%d);\n", self->framebuffer->obj);
    s += sprintf(s, "glUseProgram(program%d);\n", self->program->obj);
    s += sprintf(s, "glBindVertexArray(vertex_array%d);\n", self->vertex_array->obj);

    for (int i = 0; i < self->descriptor_set_buffers->buffers; ++i) {
        int buffer = self->descriptor_set_buffers->binding[i].buffer;
        int offset = self->descriptor_set_buffers->binding[i].offset;
        int size = self->descriptor_set_buffers->binding[i].size;
        s += sprintf(s, "glBindBufferRange(GL_UNIFORM_BUFFER, %d, buffer%d, %d, %d);\n", i, buffer, offset, size);
    }

    for (int i = 0; i < self->descriptor_set_images->samplers; ++i) {
        s += sprintf(s, "glActiveTexture(GL_TEXTURE%d);\n", i);
        s += sprintf(s, "glBindTexture(%s, image%d);\n", str_texture_target(self->descriptor_set_images->binding[i].target), self->descriptor_set_images->binding[i].image);
        s += sprintf(s, "glBindSampler(%d, sampler%d);\n", i, self->descriptor_set_images->binding[i].sampler);
    }

    if (self->index_type) {
        s += sprintf(s, "glDrawElementsInstanced(%s, %d, %s, %d * %d, %d);\n", str_topology(self->topology), self->vertex_count, str_format(self->index_type), self->first_vertex, self->index_size, self->instance_count);
    } else {
        s += sprintf(s, "glDrawArraysInstanced(%s, %d, %d, %d);\n", str_topology(self->topology), self->first_vertex, self->vertex_count, self->instance_count);
    }
}

void print_default_settings(char *& s) {
    s += sprintf(s, "glPrimitiveRestartIndex(-1);\n");
    s += sprintf(s, "glEnable(GL_PROGRAM_POINT_SIZE);\n");
    s += sprintf(s, "glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);\n");
    s += sprintf(s, "glEnable(GL_FRAMEBUFFER_SRGB);\n");
}

void print_blit_framebuffer(char *& s) {
    s += sprintf(s, "glDisable(GL_FRAMEBUFFER_SRGB);\n");
    s += sprintf(s, "glColorMaski(0, true, true, true, true);\n");
    s += sprintf(s, "glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);\n");
    s += sprintf(s, "glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);\n");
    s += sprintf(s, "glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);\n");
    s += sprintf(s, "glEnable(GL_FRAMEBUFFER_SRGB);\n");
}

PyObject * meth_dumps(PyObject * self, Context * ctx) {
    char * temp = (char *)malloc(1024 * 1024);
    char * ptr = temp;
    GCHeader * it;
    PyObject * key;
    GLObject * value;
    Py_ssize_t pos;

    it = ctx->gc_next;
    while (it != (GCHeader *)ctx) {
        if (Py_TYPE(it) == ctx->module_state->Buffer_type) {
            print_buffer(ptr, (Buffer *)it);
            ptr += sprintf(ptr, "\n");
        }
        it = it->gc_next;
    }

    it = ctx->gc_next;
    while (it != (GCHeader *)ctx) {
        if (Py_TYPE(it) == ctx->module_state->Image_type) {
            print_image(ptr, (Image *)it);
            ptr += sprintf(ptr, "\n");
        }
        it = it->gc_next;
    }

    pos = 0;
    while (PyDict_Next(ctx->sampler_cache, &pos, &key, (PyObject **)&value)) {
        print_sampler(ptr, value->obj, key);
        ptr += sprintf(ptr, "\n");
    }

    pos = 0;
    while (PyDict_Next(ctx->framebuffer_cache, &pos, &key, (PyObject **)&value)) {
        print_framebuffer(ptr, value->obj, key);
        ptr += sprintf(ptr, "\n");
    }

    pos = 0;
    while (PyDict_Next(ctx->vertex_array_cache, &pos, &key, (PyObject **)&value)) {
        print_vertex_array(ptr, value->obj, key);
        ptr += sprintf(ptr, "\n");
    }

    pos = 0;
    while (PyDict_Next(ctx->shader_cache, &pos, &key, (PyObject **)&value)) {
        PyObject * decoded = PyObject_CallMethod(PyTuple_GetItem(key, 0), "decode", NULL);
        if (!decoded) {
            return NULL;
        }
        PyObject * compact = PyObject_CallMethod(regex, "sub", "(ssNiO)", "\\s*\\n\\s*", "\n", decoded, 0, PyObject_GetAttrString(regex, "M"));
        if (!compact) {
            return NULL;
        }
        PyObject * src = PyObject_CallMethod(json, "dumps", "(N)", compact);
        if (!src) {
            return NULL;
        }
        print_shader(ptr, src, value->obj, PyLong_AsLong(PyTuple_GetItem(key, 1)));
        Py_DECREF(src);
        ptr += sprintf(ptr, "\n");
    }

    pos = 0;
    while (PyDict_Next(ctx->program_cache, &pos, &key, (PyObject **)&value)) {
        int vertex_shader = ((GLObject *)PyDict_GetItem(ctx->shader_cache, PyTuple_GetItem(key, 0)))->obj;
        int fragment_shader = ((GLObject *)PyDict_GetItem(ctx->shader_cache, PyTuple_GetItem(key, 1)))->obj;
        print_program(ptr, value->obj, vertex_shader, fragment_shader);
        ptr += sprintf(ptr, "\n");
    }

    print_default_settings(ptr);
    ptr += sprintf(ptr, "\n");

    it = ctx->gc_next;
    while (it != (GCHeader *)ctx) {
        if (Py_TYPE(it) == ctx->module_state->Pipeline_type) {
            print_pipeline(ptr, (Pipeline *)it);
            ptr += sprintf(ptr, "\n");
        }
        it = it->gc_next;
    }

    print_blit_framebuffer(ptr);
    PyObject * res = PyUnicode_FromStringAndSize(temp, ptr - temp);
    free(temp);
    return res;
}

PyMethodDef module_methods[] = {
    {"dumps", (PyCFunction)meth_dumps, METH_O, NULL},
    {},
};

PyModuleDef module_def = {PyModuleDef_HEAD_INIT, "zengl_export", NULL, -1, module_methods};

extern "C" PyObject * PyInit_zengl_export() {
    json = PyImport_ImportModule("json");
    if (!json) {
        return NULL;
    }
    regex = PyImport_ImportModule("re");
    if (!regex) {
        return NULL;
    }
    PyObject * module = PyModule_Create(&module_def);
    return module;
}
