import { globby } from 'globby'
import fs from 'fs-extra'
import {
    resolve,
} from 'path'

const out = 'dist'

const dir = out

const files = await globby([
    '**/*',
    '!*.manifest'
], {
    cwd: dir,
})

const file = 'cache.manifest'

const manifest = `CACHE MANIFEST
CACHE:
${file}
${files.sort().join('\n')}
NETWORK:
*
# Date: ${new Date().toUTCString()}
`

await fs.outputFile(`${dir}/${file}`, manifest)

const index = `${dir}/index.html`

const html = await fs.readFile(index, 'utf8')

const output = html.replace(/<html/, `<html manifest="${file}"`)

await fs.outputFile(index, output)
