/*
 * cst_plugins.c
 * 
 * Copyright 2017 Sergio Oller <sergioller@gmail.com>
 * All rights reserved.
 */
/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the  nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
 /* A plugin system for mimic */

#include "mimic_core_config.h"
#ifdef MIMIC_ENABLE_PLUGINS

#include "cst_alloc.h"
#include "cst_error.h"
#include "cst_string.h"
#include "cst_plugins.h"
#include "mimic.h"
#include <string.h>
#include <stdio.h>

#ifndef MIMIC_PLUGIN_DIR
#error Missing MIMIC_PLUGIN_DIR definition
#endif

static const char* mimic_plugin_dir = MIMIC_PLUGIN_DIR;
const char* mimic_get_plugin_dir()
{
  const char* plugindir = getenv("MIMIC_PLUGIN_DIR");
  if (plugindir != NULL)
  {
    return plugindir;
  } else
  {
    return mimic_plugin_dir;
  }
}

#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)) || defined(__CYGWIN__))
  /* UNIX-style OS, or CYGWIN */
  #define _POSIX_C_SOURCE 200809L
  #include <unistd.h>
  #if defined(_POSIX_VERSION)
    /* POSIX compliant */
    #include <dlfcn.h>
    #include <dirent.h>
    mimic_plugin_handler_t* mimic_plugin_load(mimic_context *ctx, const char *filename)
    {
        void    *handle;
        mimic_plugin_handler_t *plug_hdl = NULL;

        /* open the needed object */
        handle = dlopen(filename, RTLD_LAZY);
        if (handle == NULL)
        {
            cst_errmsg("Cannot load %s: %s", filename, dlerror ());
            return NULL;
        }
        mimic_plugin_t* plugin = dlsym(handle, "mimic_plugin");
        char *result = dlerror();
        if (result)
        {
              cst_errmsg("Cannot find mimic_plugin in %s: %s\n", filename, result);
              return NULL;
        }
        plug_hdl = cst_alloc(mimic_plugin_handler_t, 1);
        if (plug_hdl == NULL)
        {
              cst_errmsg("Memory allocation error");
              return NULL;
        }
        plug_hdl->handle = handle;
        plug_hdl->plugin = plugin;
        plugin->mimic_init(ctx);
        return plug_hdl;
    }
    int mimic_plugin_unload(mimic_context *ctx, mimic_plugin_handler_t *plug_hdl)
    {
        plug_hdl->plugin->mimic_exit(ctx);
        dlclose(plug_hdl->handle);
        cst_free(plug_hdl);
        return 0;
    }
    int list_all_plugins(char ***pluginlist)
    {
       DIR *d;
       struct dirent *dir;
       size_t fn_length;
       const char* plugindir = mimic_get_plugin_dir();
       size_t prefix_length = strlen(MIMIC_PLUGIN_PREFIX);
       size_t ext_length = strlen(MIMIC_PLUGIN_EXT);
       size_t num_plugins = 0;
       size_t num_plugins_max = 16;
       *pluginlist = (char **) malloc((num_plugins_max+1)*sizeof(char*));
       if (*pluginlist == NULL)
       {
         cst_errmsg("Error allocating memory");
         return 0;
       }
       if (plugindir == NULL)
       {
          cst_errmsg("MIMIC_PLUGIN_DIR not set. Plugins not loaded");
          (*pluginlist)[num_plugins] = NULL;
          return num_plugins;
       } else
       {
          cst_errmsg("Loading plugins from: %s\n", plugindir);
       }
       d = opendir(plugindir);
       if (d != NULL)
       {
         while ((dir = readdir(d)) != NULL)
         {
           fn_length = strlen(dir->d_name);         
           if (fn_length >= prefix_length &&
               memcmp(dir->d_name, MIMIC_PLUGIN_PREFIX, prefix_length) == 0 &&
               fn_length >= ext_length &&
               memcmp(dir->d_name + fn_length - ext_length, MIMIC_PLUGIN_EXT, ext_length) == 0)
           {
               if (num_plugins < num_plugins_max)
               {
                 printf("Loading: %s\n", dir->d_name);
                 char *fullfile = cst_alloc(char, strlen(plugindir) + strlen(dir->d_name) + 2);
                 if (fullfile == NULL)
                 {
                     cst_errmsg("Error allocating memory\n");
                     closedir(d);
                     return num_plugins;
                 }
                 int ret = snprintf(fullfile, strlen(plugindir) + strlen(dir->d_name) + 2, "%s/%s", plugindir, dir->d_name);
                 if (ret < 0 || ret >= strlen(plugindir) + strlen(dir->d_name) + 2)
                 {
                     cst_errmsg("Plugin name length too large:"); /* Should not happen */
                 }
                 (*pluginlist)[num_plugins] = fullfile;
               } else
               {
                 cst_errmsg("Maximum number of plugins reached\n"); /* FIXME: Make it dynamic */
                 closedir(d);
                 return num_plugins_max;
               }
               num_plugins++;
           }
         }
         closedir(d);
       } else
       {
           cst_errmsg("Could not open plugin directory: '%s'\n", plugindir);
       }
      (*pluginlist)[num_plugins] = NULL;
      return num_plugins;
    }
  #else
    #error "UNIX-style OS without POSIX compliance, can't compile plugin support"
  #endif
#elif defined(_WIN64) || defined(_WIN32)
    #include <windows.h>
    /* Microsoft Windows (64-bit) */
    /* Microsoft Windows (32-bit) */
    mimic_plugin_handler_t* mimic_plugin_load(mimic_context *ctx, const char *filename)
    {
        mimic_plugin_handler_t *plug_hdl = NULL;
        HINSTANCE handle = NULL;
        handle = LoadLibrary(filename);
        if (handle == NULL)
        {
            cst_errmsg("Cannot load %s: %s\n", filename);
            return NULL;
        }
        mimic_plugin_t* plugin = (mimic_plugin_t*)GetProcAddress(handle, "mimic_plugin");
        if (plugin == NULL)
        {
            cst_errmsg("Cannot find symbol mimic_plugin in file: %s\n", filename);
            return NULL;
        }
        plug_hdl = cst_alloc(mimic_plugin_handler_t, 1);
        if (plug_hdl == NULL)
        {
              cst_errmsg("Memory allocation error\n");
              return NULL;
        }
        plug_hdl->handle = handle;
        plug_hdl->plugin = plugin;
        plugin->mimic_init(ctx);
        return plug_hdl;
    }
    int mimic_plugin_unload(mimic_context *ctx, mimic_plugin_handler_t *plug_hdl)
    {
        plug_hdl->plugin->mimic_exit(ctx);
        FreeLibrary(plug_hdl->handle);
        cst_free(plug_hdl);
        return 0;
    }
    int list_all_plugins(char ***pluginlist)
    {
      WIN32_FIND_DATA fdFile;
      HANDLE hFind = NULL;
      const char *plugdir = mimic_get_plugin_dir();
      size_t num_plugins = 0;
      size_t num_plugins_max = 16;
      *pluginlist = (char **) malloc((num_plugins_max+1)*sizeof(char*));
      if (*pluginlist == NULL)
      {
        cst_errmsg("Error allocating memory\n");
        return 0;
      }

      char *sPath = malloc(strlen(plugdir) + strlen(MIMIC_PLUGIN_PREFIX) + strlen(MIMIC_PLUGIN_EXT) + 8);

    sprintf(sPath, "%s\\%s*.%s", plugdir, MIMIC_PLUGIN_PREFIX, MIMIC_PLUGIN_EXT);

    if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        cst_errmsg("Path not found: [%s]\n", plugdir);
        return 0;
    }

    do
    {
        if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
         continue;
        }
        //Build up our file path using the passed in
        //  [sDir] and the file/foldername we just found:
        if (num_plugins < num_plugins_max)
        {
          *pluginlist[num_plugins] = malloc(strlen(plugdir) + strlen(fdFile.cFileName) + 2);
          sprintf(*pluginlist[num_plugins], "%s\\%s", plugdir, fdFile.cFileName);
          num_plugins++;
        } else
        {
          cst_errmsg("Maximum number of loaded plugins reached\n");/* FIXME: Dynamic maximum */
          FindClose(hFind);
          free(sPath);
          return num_plugins_max;
        }
    } while(FindNextFile(hFind, &fdFile)); //Find the next file.

    FindClose(hFind);
    free(sPath);
    return num_plugins;
    }
#endif

int mimic_plugins_init(mimic_context *ctx)
{
  size_t i;
  char **plugin_files = NULL;
  if (ctx->plugins != NULL)
    return 0;
  int num_plugins = list_all_plugins(&plugin_files);
  ctx->plugins = cst_alloc(mimic_plugin_handler_t*, num_plugins+1);
  for (i = 0; i<num_plugins; i++)
  {
    ctx->plugins[i] = mimic_plugin_load(ctx, plugin_files[i]);
    free(plugin_files[i]);
  }
  ctx->plugins[num_plugins] = NULL;
  free(plugin_files);
  return 0;
}

void mimic_plugins_exit(mimic_context *ctx)
{
  size_t i=0;
  if (ctx->plugins != NULL)
  {
    for (i=0;ctx->plugins[i] != NULL;i++)
    {
      mimic_plugin_unload(ctx, ctx->plugins[i]);
    }
    cst_free(ctx->plugins);
    ctx->plugins = NULL;
  }
}

#else /* No plugins */

int mimic_plugins_init(mimic_context *ctx)
{
  return 0;
}

void mimic_plugins_exit(mimic_context *ctx)
{
  return;
}

#endif

