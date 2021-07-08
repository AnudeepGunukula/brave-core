/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */
/* global window */

import { createReducer } from 'redux-act'
import * as Actions from '../actions/wallet_page_actions'
import { PageState, AssetOptionType, AssetPriceTimeframe } from '../../constants/types'
import { WalletCreatedPayloadType, RecoveryWordsAvailablePayloadType, SelectAssetPayloadType } from '../constants/action_types'

const defaultState: PageState = {
  hasInitialized: false,
  showRecoveryPhrase: false,
  invalidMnemonic: false,
  selectedTimeline: AssetPriceTimeframe.OneDay,
  selectedAsset: undefined,
  selectedAssetPrice: undefined,
  selectedAssetPriceHistory: [],
  portfolioPriceHistory: [],
  userAssets: ['1', '2'],
  isFetchingPriceHistory: false
}

const reducer = createReducer<PageState>({}, defaultState)

reducer.on(Actions.walletCreated, (state: PageState, payload: WalletCreatedPayloadType) => {
  return {
    ...state,
    mnemonic: payload.mnemonic
  }
})

reducer.on(Actions.recoveryWordsAvailable, (state: PageState, payload: RecoveryWordsAvailablePayloadType) => {
  return {
    ...state,
    mnemonic: payload.mnemonic
  }
})

reducer.on(Actions.walletSetupComplete, (state: PageState) => {
  const newState = { ...state }
  delete newState.mnemonic
  return newState
})

reducer.on(Actions.walletBackupComplete, (state: PageState) => {
  const newState = {
    ...state,
    showRecoveryPhrase: false
  }
  delete newState.mnemonic
  return newState
})

reducer.on(Actions.showRecoveryPhrase, (state: PageState, payload: boolean) => {
  return {
    ...state,
    showRecoveryPhrase: payload
  }
})

reducer.on(Actions.hasMnemonicError, (state: PageState, payload: boolean) => {
  return {
    ...state,
    invalidMnemonic: payload
  }
})

reducer.on(Actions.updateSelectedAsset, (state: PageState, payload: AssetOptionType) => {
  return {
    ...state,
    selectedAsset: payload
  }
})

reducer.on(Actions.updatePriceInfo, (state: PageState, payload: SelectAssetPayloadType) => {
  const history = payload.priceHistory ? payload.priceHistory.values : []
  return {
    ...state,
    selectedAssetPrice: payload.priceHistory ? {
      usd: payload.price,
      btc: 0,
      change24Hour: 0
    } : undefined,
    selectedAssetPriceHistory: history,
    selectedTimeline: payload.timeFrame,
    isFetchingPriceHistory: false
  }
})

reducer.on(Actions.setIsFetchingPriceHistory, (state: PageState, payload: boolean) => {
  return {
    ...state,
    isFetchingPriceHistory: payload
  }
})

export default reducer
