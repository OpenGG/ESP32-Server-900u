import { defineConfig } from 'vite'
import { viteStaticCopy } from 'vite-plugin-static-copy'
import { viteSingleFile } from "vite-plugin-singlefile"

import PATH from 'path'

const {
    RELEASE,
    version,
} = process.env

const out = RELEASE ? 'docs' : 'dist'

export default defineConfig({
    root: PATH.resolve(`./src/${version}`),
    base: '',
    build: {
        polyfillModulePreload: false,
        emptyOutDir: true,
        outDir: PATH.resolve(`./${out}/${version}`)
    },
    plugins: [
        viteStaticCopy({
            flatten: true,
            targets: [
                {
                    src: PATH.resolve(`./src/${version}/static/*`),
                    dest: '.'
                }
            ]
        }),
        viteSingleFile(),
    ],
})