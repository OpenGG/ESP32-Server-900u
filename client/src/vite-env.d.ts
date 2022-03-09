/// <reference types="vite/client" />

declare module 'vite-i18n-resources'

type ValueOf<T> = T[keyof T];

declare type I18n = {
  (t: string): string;
  <T extends keyof ValueOf<M>, M extends Record<string, Record<string, string>>>(t: T, m: M): string;
}

declare const $t: I18n
