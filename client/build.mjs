import Handlebars from 'handlebars';
import fs from 'fs-extra'
import path from 'path'
import menuConfig from './src/menuConfig.mjs';

const handlebars = Handlebars.create()

const binSrc = path.resolve('./src/bin')
const binDst = path.resolve('./dist')
const source = path.resolve('./src/index.hbs')
const output = path.resolve('./dist/index.html')
const base = path.dirname(source)

const buildTemplate = async () => {
    const template = await fs.readFile(source, 'utf8')

    let collectEmbeds = new Set()

    handlebars.registerHelper('embed', (src) => {
        collectEmbeds.add(src)
        return ''
    });

    const compiled = handlebars.compile(template, {
        strict: true,
    });

    compiled({})

    const embedMaps = new Map()

    await Promise.all([...collectEmbeds].map(async src => {
        const content = await fs.readFile(
            path.resolve(base, src),
            'utf8'
        )
        embedMaps.set(src, content)
    }))

    handlebars.registerHelper('embed', (src) => {
        if (embedMaps.has(src)) {
            return ` */\n${embedMaps.get(src)}\n/* `
        }

        throw new Error(`Not found in 1st pass: ${src}`)
    })

    return compiled({
        menuConfig
    })
}

const out = await buildTemplate()

await fs.outputFile(output, out)

await fs.copy(binSrc, binDst)