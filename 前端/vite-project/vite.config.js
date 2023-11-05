import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import AutoImport from 'unplugin-auto-import/vite'
import Components from 'unplugin-vue-components/vite'
import { ElementPlusResolver } from 'unplugin-vue-components/resolvers'

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [vue(),
  AutoImport({
    resolvers: [ElementPlusResolver()],
  }),
  Components({
    resolvers: [ElementPlusResolver()],
  }),],
  base:'./',
  server: {
    proxy: {
      '/api': {
        changeOrigin: true, //允许跨域
        pathRewrite: {
          '^/api': '' //请求的时候使用这个api就可以
        },
        target: 'http://192.168.9.77:8080',
      },
      '/sql': {
        changeOrigin: true, //允许跨域
        pathRewrite: {
          '^/sql': '' //请求的时候使用这个api就可以
        },
        target: 'http://192.168.9.77:8081',
      }
    },
    /*proxy: {
      '/sql': {
        changeOrigin: true, //允许跨域
        pathRewrite: {
          '^/sql': '' //请求的时候使用这个api就可以
        },
        target: 'http://192.168.9.77:8081',
      }
    }*/
  }
})
