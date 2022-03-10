import { defineConfig } from 'vite'
import { viteStaticCopy } from 'vite-plugin-static-copy'
import { viteSingleFile } from "vite-plugin-singlefile"

import PATH from 'path'


const out = 'dist'

export default defineConfig({
    root: PATH.resolve(`./src/`),
    base: '',
    build: {
        minify: false,
        polyfillModulePreload: false,
        emptyOutDir: true,
        outDir: PATH.resolve(`./${out}`)
    },
    plugins: [
        viteStaticCopy({
            flatten: true,
            targets: [
                {
                    src: PATH.resolve(`./src/static/*`),
                    dest: '.'
                }
            ]
        }),
        viteSingleFile(),
    ],
})