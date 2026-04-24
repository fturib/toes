# TOES - Progressive Web App (PWA) for Android

This folder contains the PWA configuration files for the TOES application. The webapp is now ready to be installed as an app on Android devices.

## How to Install on Android

### Using Chrome (Recommended)

1. Open Chrome on your Android phone
2. Navigate to: `https://toes-backend-202431845326.us-central1.run.app` (or wherever your webapp is deployed)
3. Tap the **menu button** (three dots) in the top-right corner
4. Select **"Install app"** or **"Add to Home screen"**
5. Follow the prompts to confirm installation
6. The app will be added to your home screen as an icon named "TOES"

### Using Firefox

1. Open Firefox on your Android phone
2. Navigate to the webapp URL
3. Long-press the address bar
4. Select **"Install"**
5. Follow the prompts

## Features

✅ **Offline Support** - App caches data and works offline
✅ **Fast Loading** - Instant app launch with cached resources
✅ **App Icon** - Appears on home screen like a native app
✅ **Full Screen** - Launches without browser UI
✅ **Notifications** - Can receive push notifications (optional setup)
✅ **Share** - Can share data with other apps

## What's New (PWA Setup)

### Files Added

- `public/manifest.json` - App metadata and configuration
- `public/service-worker.js` - Handles caching and offline functionality
- Updated `index.html` - Service worker registration

### Caching Strategy

- **Static Assets** (CSS, JS, images): Cached on first visit, serve from cache
- **API Calls** (Firestore, Spreadsheet): Try network first, fallback to cache
- **Automatic Updates**: Checks for updates when app is refreshed

## Building for Deployment

Build the app for production:

```bash
cd toes-webapp
npm run build
```

Deploy the `dist/` folder to your Cloud Run backend or web server.

## Testing PWA Locally

### Using Chrome DevTools (Desktop)

1. Run `npm run dev` in toes-webapp
2. Open DevTools (F12)
3. Go to **Application** tab → **Manifest**
4. You can test the manifest and service worker here

### Testing on Android

1. Run your local dev server
2. Use **chrome://inspect** to connect your Android phone
3. Or use `npm run build && npm run preview` to test the production build locally
4. Access from your Android phone via local network (e.g., `http://192.168.x.x:4173`)

## Troubleshooting

### App won't install

- Ensure the site is served over **HTTPS** (PWAs require HTTPS)
- Clear browser cache and try again
- Make sure `manifest.json` is accessible

### App not updating

- Force refresh the app (pull down to refresh)
- Clear app data in Settings → Apps → TOES
- Reinstall the app

### Offline features not working

- Check that service worker is registered in DevTools
- Ensure the service-worker.js file is at the root (public folder)
- Clear browser cache and reinstall

## Requirements

- **Android 5.0+** (for Chrome PWA support)
- **HTTPS** (required by modern browsers)
- **Chrome, Firefox, or Samsung Internet** browser

## Next Steps (Optional)

For even better Android integration, consider:

1. **Google Play Store Distribution** - Use Bubblewrap to convert PWA to APK
2. **Push Notifications** - Implement Web Push API
3. **Custom Icons** - Create professional app icons
4. **Splash Screens** - Add custom splash screen on launch

---

**Questions?** Check Chrome's PWA documentation or your browser's PWA install prompts.
