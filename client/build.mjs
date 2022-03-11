import fs from 'fs-extra'
import assert from 'assert'
import { renderMenu } from './src/menu.mjs'
import path from 'path'

const binSrc = path.resolve('./src/bin')
const binDst = path.resolve('./dist')
const source = path.resolve('./src/index.html')
const output = path.resolve('./dist/index.html')
const base = path.dirname(source)


const compose = (...fns) => async (arg) => {
    let current = arg

    for (let i = fns.length - 1; i > -1; i--) {
        const fn = fns[i]
        current = await fn(current)
    }

    return current
}

const readIndex = () => fs.readFile(source, 'utf8')

const withMenu = async (html) => {
    const menuHTML = await renderMenu()

    return html.replace('__MENU_HTML__', menuHTML)
}

const embedScripts = (dir, regex, template) => async (html) => {
    const set = new Set()

    for (; ;) {
        const matches = regex.exec(html);

        if (!matches) {
            break;
        }

        set.add(matches[1]);
    }

    const files = [...set];

    const contents = await Promise.all(files.map(f => fs.readFile(path.resolve(dir, f), 'utf8')))

    return html.replace(regex, (_, g1) => {
        const index = files.indexOf(g1)

        assert.notEqual(index, -1)

        return template.replace('__CONTENT__', contents[index])
    })
}

const embedJS = embedScripts(
    base,
    /<script src="([^"]+)"><\/script>/g,
    `<script>
__CONTENT__
</script>`
);

const embedCSS = embedScripts(
    base,
    /<link [^>]*href="([^"]+)"[^>]*>/g,
    `<style>
__CONTENT__
</style>`
)

const out = await compose(
    embedCSS,
    embedJS,
    withMenu,
    readIndex
)()

await fs.outputFile(output, out)

await fs.copy(binSrc, binDst)