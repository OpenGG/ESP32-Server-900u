
type ILocale = {
    text: string,
    tips: string,
}

export type IButtonConfig = {
    locale: Record<string, ILocale>,
    primary?: boolean,
    type: string,
    url: string
}

type IPanel = IButtonConfig[]

export type IMenu = IPanel[]
